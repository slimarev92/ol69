/*******************************************************
*                                                      *
*      .oooooo.   ooooo            .ooo    .ooooo.     *
*     d8P'  `Y8b  `888'          .88'     888' `Y88.   *
*    888      888  888          d88'      888    888   *
*    888      888  888         d888P"Ybo.  `Vbood888   *
*    888      888  888         Y88[   ]88       888'   *
*    `88b    d88'  888       o `Y88   88P     .88P'    *
*     `Y8bood8P'  o888ooooood8  `88bod8'    .oP'       *
*                                                      *
*         SYSTEM PROGRAMMING: SIGNALS EX 2 (CHILD)     *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        ALEX ZILBERMAN           * 
*******************************************************/
#include <unistd.h> /* fork */
#include <sys/types.h> /* pid_t */
#include <signal.h> /* sigaction */
#include <string.h> /* string */
#include <time.h> /* nanosleep */
#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */

#include "sig.h"

volatile static sig_atomic_t g_can_send = 1;

static void UsrHandler(int signal)
{   
    g_can_send = 1;
}

static void SetVars(int *signal, 
                    pid_t *pid,
                    struct sigaction *sa,
                    struct timespec *ts)
{
    *signal = SIGUSR1;
    *pid = getppid();
    sa->sa_handler = UsrHandler;
    sigaction(SIGUSR2, sa, NULL);
    ts->tv_sec = 0;
    ts->tv_nsec = SLEEP_NANOSECONDS; 
}

static void RunPingPong()
{
    size_t i = NUM_OF_TIMES;
    pid_t pid = 0;
    struct sigaction sa = {};
    int signal = 0;   
    struct timespec ts = {0, 0}; 

    SetVars(&signal, &pid, &sa, &ts);

    while(0 < i)
    {      
        if(g_can_send)
        {
            --i;
            g_can_send = 0;

            printf("ping %lu\n---------------\n", i);

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
}

int main()
{
    RunPingPong();
    
    return 0;
}