#include <stdlib.h>
#include <stdio.h>

#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    char *writestr = argv[2];
    openlog("writer", 0, LOG_USER);

    if(argc < 2)
    {       
        syslog(LOG_ERR, "No Filename or Write String Specified");
        fprintf(stderr, "No parameters specified \n");
        closelog();
        return EXIT_FAILURE;
        
    }
    else if(filename != NULL && writestr == NULL)
    {
        syslog(LOG_ERR, "No Write String Specified");
        fprintf(stderr, "No Write String Specified for file: %s\n", argv[1]);
        closelog();
        return EXIT_FAILURE;
    }
    else if (filename !=NULL && writestr != NULL)
    {
        syslog(LOG_DEBUG, "Filename: %s, Write String: %s", filename, writestr);
        int fd  = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0700);
        if (fd == -1) 
        {
            syslog(LOG_ERR, "Failed to open file: %s", filename);
        }
        else if(fd != -1)
        { 
            syslog(LOG_DEBUG, "Writing string %s to file: %s", writestr, filename);
            write(fd, writestr, strlen(writestr));
            close(fd);
        }
    }
    closelog(); 
    return EXIT_SUCCESS;

}

