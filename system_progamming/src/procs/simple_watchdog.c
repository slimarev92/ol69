/**********************************************************
* SYSTEM PROGRAMMING: SIMPLE WATCHDOG (FORK AND SYSTEM)   *
* CODE BY:            SASHA LIMAREV                       *   
* REVIEWED BY:        MISHEL LIBERMAN                     *  
***********************************************************/
#include <unistd.h> /* fork */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait */
#include <stdio.h> /* printf */
#include <stdlib.h> /* system */

static void RunFork()
{
    pid_t child_pid = 0;

    while(1)
    {
        child_pid = fork();

        if(0 == child_pid)
        {
            execlp("./child.out", "child.out", NULL);
        }
        else
        {
            printf("Waiting for child to finish (fork)\n");
            wait(NULL);
        }
    }
}

static void RunSystem()
{
    pid_t child_pid = 0;

    while(1)
    {
        printf("Waiting for child to finish (system)\n");
        system("./child.out");
    }
}

int main()
{

    RunSystem();

    return 0;
}