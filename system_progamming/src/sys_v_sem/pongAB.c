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
#define SEM_NUM (23)
#define ALWAYS (1)

static int GetSem(key_t sem_key);

static int GetSem(key_t sem_key)
{
    int semid = semget(sem_key, 2, 0);

    if(-1 == semid)
    {
        puts("Ping is not running at the moment.");
        
        return -1;
    }
    
    return semid;
}

int main()
{
    int semid = 0;
    key_t sem_key = ftok("./sem.key", SEM_NUM); /* get sem set key */
    struct sembuf op_post = {1, 1, 0};
    struct sembuf op_wait = {0, -1, 0};
    size_t count = 0;

    if(-1 == sem_key)
    {
        perror("Error while generating key");

        return 1;
    }

    semid = GetSem(sem_key);

    if(-1 == semid)
    {
        return 1;
    }

    while(ALWAYS)
    {
        if(-1 == semop(semid, &op_wait, 1))
        {
              puts("Semaphore doesn't exist (which means ping is also gone)");

            return 1;
        }
        
        sleep(1);
        printf("Pong %lu!\n", count);

        ++count;        

        if(-1 == semop(semid, &op_post, 1))
        {
            puts("Semaphore doesn't exist (which means ping is also gone)");

            return 1;
        }
    }

    return 0;
}
