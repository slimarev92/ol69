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
*         REVIEWED BY:        YOAV KLEIN               * 
*******************************************************/
#include <stdio.h> /* puts */
#include <sys/types.h> /* key_t */
#include <sys/ipc.h> /* ftok */
#include <sys/sem.h> /* semctl */
#include <unistd.h> /* sleep */ 

#define READ_WRITE (0600)
#define ITERATIONS (10)
#define SEM_NUM (23)

key_t g_sem_key = 0;

static int CreateOrGetSem(key_t g_sem_key);

static int CreateOrGetSem(key_t g_sem_key)
{
    int semid = semget(g_sem_key, 2, IPC_CREAT | READ_WRITE);

    if(-1 == semid)
    {
        perror("error");
        
        return -1;
    }
    
    return semid;
}

int main()
{
    int semid = 0;
    key_t g_sem_key = ftok("./sem.key", SEM_NUM); /* get sem set key */
    struct sembuf op_post = {0, 1, 0};
    struct sembuf op_wait = {1, -1, 0};
    size_t count = 0;

    if(-1 == g_sem_key)
    {
        perror("Error while generating key");

        return 1;
    }

    semid = CreateOrGetSem(g_sem_key);

    if(-1 == semid)
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
            puts("op_post error:");

            return 1;
        }

        if(-1 == semop(semid, &op_wait, 1))
        {
            perror("op_wait error:");
            
            return 1;
        }
        
    }

    puts("Ping is done");

    if(-1 == semctl(semid, 0, IPC_RMID))
    {
        perror("error 10:");
    }

    return 0;
}
