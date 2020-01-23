/*******************************************************
*                    .--.                              *
*                    |__| .-------.                    *
*                    |=.| |.-----.|                    *
*                    |--| || >$  ||                    *
*                    |  | |'-----'|                    *
*                     |__|~')_____('                    *
*                                                      *
*          SYSTEM PROGRAMMING: SIGNALS EX 1            *
*          CODE BY:            SASHA LIMAREV           *
*          REVIEWED BY:        ALEX ZILBERMAN          * 
*                                                      *
*******************************************************/
#include <unistd.h> /* fork */
#include <sys/wait.h> /* wait */
#include <sys/types.h> /* pid_t */
#include <signal.h> /* sigaction */
#include <stdio.h> /* printf */
#include <string.h> /* string */
#include <time.h> /* nanosleep */
#include <stdlib.h> /* exit */

#include "sig.h"

volatile static sig_atomic_t g_can_send = 0;

static void UsrHandler(int signal)
{   
    g_can_send = 1;
}
  
static void SetVars(int *signal,
                    pid_t *pid,
                    char **message,
                    struct sigaction *sa,
                    struct timespec *ts)
{
    ts->tv_sec = 0;
    ts->tv_nsec = SLEEP_NANOSECONDS; 

    if(0 == *pid) /* I'm the child */
    {
        *signal = SIGUSR1;
        *pid = getppid();
        *message = "pong";
        sa->sa_handler = UsrHandler;
        sigaction(SIGUSR2, sa, NULL);
    }
    else /* I'm the parent */
    {
        *signal = SIGUSR2; 
        *message = "ping";
        sa->sa_handler = UsrHandler;
        sigaction(SIGUSR1, sa, NULL);
        g_can_send = 1;
    }
}

static void RunPingPong()
{
    size_t i = NUM_OF_TIMES;
    pid_t pid = 0;
    struct sigaction sa = {};
    static char *message = NULL;
    int signal = 0;    
    struct timespec ts = {0, 0};

    pid = fork();

    SetVars(&signal, &pid, &message, &sa ,&ts);
    
    while(0 < i)
    {     
        if(g_can_send)
        {
            --i;
            g_can_send = 0;

            printf("%s %lu\n---------------\n", message, i);

            if(0 != kill(pid, signal))
            {
                exit(-1);
            }
        }   
        else 
        {
            /* sleep for 0.0001 seconds or until a signal arrives */
            nanosleep(&ts, NULL);
        }
    }

    wait(NULL);
}

int main()
{
    RunPingPong();
    
    return 0;
}