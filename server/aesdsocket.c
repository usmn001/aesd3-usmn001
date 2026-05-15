#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include<signal.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define __USE_MISC
#include <netdb.h>

const char *pMesaage = "Hello";

int socketCreate(struct addrinfo *res)        // Our Own Function Creating Socket 
{
	struct addrinfo *rp = NULL;
	int hSocket = -1;
	for (rp = res; rp != NULL; rp = rp->ai_next) 
	{
    	hSocket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (hSocket == -1)
        	continue;    
    }    
	return hSocket;
}

int bindCreatedSocket(int hSocket,struct addrinfo *res) 
{

int iRetval = -1;
iRetval = bind(hSocket, res->ai_addr,res->ai_addrlen);
return iRetval;
}

volatile sig_atomic_t exit_requested = 0;

void termination_handler(int signum) 
{
   
	exit_requested = -1;
	//printf("Received Termination Signal\n ");
	syslog(LOG_DEBUG,"Caught signal, exiting");
}


int main(int argc, char *argv[])
{
    char *mode = argv[1];
	openlog("aesdsocket", 0, LOG_USER);    
    char *filename = "/var/tmp/aesdsocketdata";
	ssize_t recv_bytes = 0;
 	struct sigaction new_action;

    new_action.sa_handler = termination_handler;
    sigemptyset(&new_action.sa_mask); //Specifying No Signal Should Be Blocked During Exection of Termination_Handler
    new_action.sa_flags = 0;    // specifies a set of flags which modify the behavior of signals.
    
    
    if (sigaction(SIGINT, &new_action, NULL) == -1) 
	{
        perror("Error setting up signal handler");
        return 1;
    }
    
	if (sigaction(SIGTERM, &new_action, NULL) == -1) 
	{
        perror("Error setting up signal handler");
        return 1;
    }


	int socket_desc = 0; 
	int client_sock = 0; 
	int s = 0;
	
	char msg_from_client[1024] = {0};
	
	char client_ip[NI_MAXHOST];
    char service[NI_MAXSERV];
    
	struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

	struct addrinfo *res = NULL;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family   = AF_INET;       // IPv4   /* Internet Address Family */
	hints.ai_socktype = SOCK_STREAM;   // TCP
	hints.ai_flags    = AI_PASSIVE;    // For bind()

	s=getaddrinfo(NULL, "9000", &hints, &res);
	if(s!=0)
	{
		 syslog(LOG_ERR, "getaddrinfo: %s",gai_strerror(s));
		 perror("Get addrinfo failed");
		 exit(EXIT_FAILURE);
	}
    socket_desc = socketCreate(res);

	if(socket_desc!=-1)
	{
		//printf("SOCKET CREATED SUCCESSFULLY\n");
		syslog(LOG_DEBUG,"SOCKET DONE");
	}

	if(bindCreatedSocket(socket_desc,res)!=0)     // Bind System Call Assigns an IP address to socket file descriptor.
	{
        syslog(LOG_DEBUG,"BIND FAILED");
	}
	//printf("BIND DONE SUCCESSFULLY\n");
	syslog(LOG_DEBUG,"BIND DONE");
	freeaddrinfo(res);	

	
//Listen
listen(socket_desc,3);
if(argc==2 && strcmp(mode,"-d")==0)
	{
    	pid_t pid = fork(); 
        
		if(pid<0)
		{
			syslog(LOG_ERR,"FORK FAILED");
			exit(EXIT_FAILURE);
		}
		else if(pid>0)    // In Parent Process
		{
			exit(EXIT_SUCCESS);   // Parent Exiting
		}

        if(setsid()<0)   //Child Process creates a session and sets the process group ID
		{
			syslog(LOG_ERR,"setsid() failed"); 
			exit(EXIT_FAILURE);
		}
		close(STDIN_FILENO);         // scanf and other statements won´t work in daemon child process.
    	close(STDOUT_FILENO);        // printf will not work in daemon child process.
    	close(STDERR_FILENO);        // perror statement will not work in daemon child process.
	}


    while(exit_requested!=-1)
    {
	    syslog(LOG_DEBUG, "Waiting for a Connection");        

	    // Accept connection from an incoming client_addr, accept is returning a new socket descriptor
		socklen_t clientLen = sizeof(client_addr);
	    client_sock = accept(socket_desc,(struct sockaddr *)&client_addr,&clientLen);
		
		if(client_sock<0)
	    {
 	        if (errno == EINTR && exit_requested)
			{
            break;
			}
			else
			continue;
	    }
		s = getnameinfo((struct sockaddr *)&client_addr,client_len,client_ip,sizeof(client_ip),service,sizeof(service),NI_NUMERICHOST | NI_NUMERICSERV);
		if(s==0)
		{
			syslog(LOG_DEBUG, "Accepted Connection From Client %s",client_ip);
			//printf("Connection Accepted From Client %s \n",client_ip);
		}

        int fd  = open(filename, O_RDWR | O_CREAT | O_APPEND, 0700);

        if (fd == -1) 
        {
            syslog(LOG_ERR, "Failed to open file: %s with error %s", filename,strerror(errno));
			exit(EXIT_FAILURE);
        }

        size_t packet_size = 0;
		ssize_t wr_data = 0;
		ssize_t sent_data = 0;
		ssize_t total_sent_data = 0;
		char *packet_buffer = NULL;
		while(1)
		{
            // Receive a Reply from the client_addr
	    	
			recv_bytes = recv(client_sock,msg_from_client,sizeof(msg_from_client),0);

			if(recv_bytes<0)
			{
				syslog(LOG_ERR,"RECV system call failed %s",strerror(errno));
				exit(EXIT_FAILURE);
			}
			else if(recv_bytes>0)
        	{
				syslog(LOG_DEBUG, "Received %zu Bytes From Client %s", recv_bytes,client_ip);
				//printf("Total %zu Bytes Received From Client %s \n", recv_bytes,client_ip);
			}
			else if(recv_bytes==0)
			{
				break;
			}

            char *tmp_buffer = (char*)realloc(packet_buffer,packet_size+recv_bytes);
           	
			if(tmp_buffer==NULL)
		    {
				syslog(LOG_ERR,"Memory Allocation For tmp_buffer failed %s",strerror(errno));
				free(packet_buffer);
				packet_buffer=NULL;
				exit(EXIT_FAILURE);
		    }

			packet_buffer = tmp_buffer;
           
			memcpy(tmp_buffer+packet_size,msg_from_client,recv_bytes); 
            
			packet_size+=recv_bytes;

            if(memchr(msg_from_client,'\n', recv_bytes) != NULL)
            {
				wr_data=write(fd, tmp_buffer,packet_size);
                if(wr_data<0)
				{
					syslog(LOG_ERR,"Write To File Failed With Error %s",strerror(errno));	
				} 
				syslog(LOG_DEBUG,"Writing %zu Bytes To File %s",wr_data,filename);
				//printf("Writing %zu Bytes To File %s\n",wr_data,filename);
				break;
			}	      
		}
		free(packet_buffer);
        off_t filesize;
		filesize = lseek(fd,0,SEEK_END);
        
		char *read_buffer = (char*)malloc(filesize);
        if(read_buffer!=NULL)
		{
		    ssize_t rd_data;
			lseek(fd, 0, SEEK_SET);
			rd_data=read(fd,read_buffer,filesize);
			if(rd_data==-1)
			{
				syslog(LOG_ERR,"Reading file has failed with %s",strerror(errno));
				//printf("Reading file has failed with %s\n",strerror(errno));
				exit(EXIT_FAILURE);
			}
			else if(rd_data==0)
			{
				syslog(LOG_DEBUG,"Reached the end of file");
				//printf("Reached the end of file \n");
			}
			else if(rd_data>0)
			{
				/*printf("Displaying Data Read From File\n");

				for(size_t i = 0;i<rd_data;i++)
				{
					printf("%c",read_buffer[i]);
				}*/

				sent_data=send(client_sock,read_buffer,rd_data,0);
        
				if(sent_data<0)
				{
					syslog(LOG_ERR,"DATA TRANSMISSION FAILED ");
				}
				else if(sent_data>0)
				{
					//printf("\nSent %zu Data Bytes To Client\n",sent_data);
                    syslog(LOG_DEBUG,"Sent %zu Data Bytes To Client",sent_data);
				}
				total_sent_data +=sent_data;
			}	
		}
		free(read_buffer);
        close(fd);	
	}

	close(socket_desc);
	syslog(LOG_DEBUG,"CLOSED CONNECTION FROM %s",client_ip);
	remove("/var/tmp/aesdsocketdata");
	closelog();   
}
			
			
			
			
	