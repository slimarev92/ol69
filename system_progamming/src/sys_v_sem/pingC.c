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
*         SYSTEM PROGRAMMING: SYS V SEMAPHORE          *
*         CODE BY:            SASHA LIMAREV            *
*******************************************************/
#include <stdio.h> /* puts */
#include <sys/types.h> /* key_t */
#include <sys/ipc.h> /* ftok */
#include <sys/sem.h> /* semctl */
#include <unistd.h> /* sleep */ 
#include <stdlib.h> /* atexit */
#include <signal.h> /* sigaction */

#include "constants.h"

static key_t g_sem_key = 0;

static int CreateSem();
static void RemSem();
static int SetSigaction();
static void SignalHanler(int signal);
static int Init(int *semid);
static int MainLoop();

static void SignalHanler(int signal)
{
    (void)signal;

    exit(1);
}

static void RemSem()
{
    int semid = semget(g_sem_key, 2, 0);;

    if(-1 == semid)
    {
        return;
    }

    semctl(semid, 0, IPC_RMID);
}

static int CreateSem()
{
    int semid = 0;

    semid = semget(g_sem_key, 2, IPC_CREAT | READ_WRITE);

    if(-1 == semid)
    {
        perror("Error while getting semaphore");
        
        return -1;
    }
    
    return semid;
}

static int SetSigaction()
{
    struct sigaction sigint_action;

    if(-1 == atexit(RemSem))
    {
        perror("Error while registering atexit handler");

        return 1;
    }
    
    sigint_action.sa_handler = SignalHanler;

    if(-1 == sigaction(SIGINT, &sigint_action, NULL))
    {
        perror("Error while resigstering SIGINT handler");

        return 1;
    }

    return 0;
}

static int Init(int *semid)
{ 
    g_sem_key = ftok("./sem.key", SEM_NUM);

    if(-1 == g_sem_key)
    {
        perror("Error while generating key");

        return 1;
    }

    *semid = CreateSem();

    if(-1 == *semid)
    {
        return 1;
    }

    return 0;
}

static int MainLoop()
{
    int semid = 0;
    size_t count = 0;    
    struct sembuf op_post = {PONG_SEM, POST, 0};
    struct sembuf op_wait = {PING_SEM, WAIT, 0};

    if(0 != Init(&semid) || 0 != SetSigaction())
    {
        return 1;
    }   

    while(ITERATIONS > count)
    {
        sleep(1);        
        printf("Ping! %lu\n", count);

        ++count;

        if(-1 == semop(semid, &op_post, 1))
        {
            puts("Semaphore doesn't exist (which means pong is also gone)");

            return 1;
        }

        if(-1 == semop(semid, &op_wait, 1))
        {
            puts("Semaphore doesn't exist (which means pong is also gone)");

            return 1;
        }
    }

    return 0;
}

int main()
{
    if(0 != MainLoop())
    {
        return 1;
    }

    puts("Ping is done");

    exit(0);
}
