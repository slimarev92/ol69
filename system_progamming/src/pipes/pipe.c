#include <unistd.h> /* pipe */
#include <stdio.h> /* perror */
#include <sys/types.h> /* pid_t */
#include <string.h> /* strlen */
#include <stdlib.h> /* malloc */

#define READ_FD (0)
#define WRITE_FD (1)
#define STR_SIZE (50)

int main(int argc, char **argv)
{
    pid_t child_pid = 0;
    int pipefs[2] = {0, 0}; 
    char *argv_1 = NULL;

    if(2 > argc)
    {
        puts("Error: Missing command line argument");  
        return 1;
    }

    if(0 != pipe(pipefs))
    {
        perror("Error while creating pipe");
        return 1;
    }   

    child_pid = fork();

    if(-1 == child_pid) /* fork failed */
    {
        perror("Error while forking child proc");
    }
    else if(0 == child_pid) /* parent writes argv[1] to pipe */
    {
        if(-1 == close(pipefs[READ_FD]))
        {
            perror("Error while closing read end of a pipe");
            return 1;
        }

        if(-1 == write(pipefs[WRITE_FD], argv[1], strlen(argv[1])))
        {
            perror("Error while writing to pipe");
            return 1;
        }

        /* closing this pipe will put EOF on the reading side */ 
        close(pipefs[WRITE_FD]);   
    }
    else /* child reads from pipe, then prints */
    {
        if(-1 == close(pipefs[WRITE_FD]))
        {
            perror("Error while closing write end of a pipe");
        }

        argv_1 = (char *)calloc(1, sizeof(char) * STR_SIZE);
        if(NULL == argv_1)
        {
            perror("Error while allocating argv_1");
            return 1;
        }

        if(-1 == read(pipefs[READ_FD], argv_1, STR_SIZE))
        {
            perror("Error while reading from pipe");
            free(argv_1);
            close(pipefs[READ_FD]);
            return 1;
        }

        printf("%s\n", argv_1);
        free(argv_1);
        close(pipefs[READ_FD]);
    }

    return 0;
}