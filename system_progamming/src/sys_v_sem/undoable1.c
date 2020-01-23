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

static int GetSem();
static int Init(int *semid);
static int MainLoop();

static key_t g_sem_key = 0;

union semun 
{
    int val;
};

static int GetSem()
{
    int semid = semget(g_sem_key, 1, IPC_CREAT | READ_WRITE);

    if(-1 == semid)
    {
        perror("GetSem error");
        
        return -1;
    }
    
    return semid;
}

static int Init(int *semid)
{ 
    union semun sem_init_info = {1};

    g_sem_key = ftok("./sem.key", SEM_NUM);
    
    if(-1 == g_sem_key)
    {
        perror("Error while generating key");

        return 1;
    }

    *semid = GetSem();

    if(-1 == *semid)
    {
        perror("GetSem error");

        return 1;
    }

    if(-1 == semctl(*semid, 0, SETVAL, sem_init_info))
    {
        perror("semctl error");

        return 1;
    }

    return 0;
}

static int MainLoop()
{
    int semid = 0;
    struct sembuf op_wait = {0, WAIT, SEM_UNDO};

    if(0 != Init(&semid))
    {
        return 1;
    }   

    printf("Pretending to lock file...\n");
    printf("Time to run undo2\n");
    
    if(-1 == semop(semid, &op_wait, 1))
     {
        perror("semop failed:");

        return 1;
    }

    sleep(1000);

    return 0;
}

int main()
{
    if(0 != MainLoop())
    {
        return 1;
    }

   return 0;
}
