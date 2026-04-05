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

    if(filename == NULL || writestr == NULL)
    {
        syslog(LOG_ERR, "No Filename or Write String Specified");
        fprintf(stderr, "Usage: %s <filename> <writestr>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
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

    return EXIT_SUCCESS;

}


/*
int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <writestr>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    char *writestr = argv[2];

    FILE *file = fopen(filename, "w");
    openlog(NULL, 0, LOG_USER);
    if (file == NULL) 
    {
        syslog(LOG_ERR, "Failed to open file: %s", filename);
        return EXIT_FAILURE;
    }
    else if(file !=NULL)
    { 
        syslog(LOG_DEBUG, "Writing string %s to file: %s", writestr, filename);
        fprintf(file, "%s", writestr);
        fclose(file);
    }

    return EXIT_SUCCESS;
}
*/