#include <sys/ipc.h> /* shmget */
#include <sys/shm.h> /* shmget */
#include <stdio.h> /* perror */
#include <sys/ipc.h> /* shmctl */

#define READ_WRITE (0600)

int main()
{
    key_t share_key = 0;
    int shmid = 0;
    char *mem_block = NULL;
    size_t i = 0;

    share_key = ftok("./sysv.key", 23);
    if(-1 == share_key)
    {
        perror("Error while genrating key");
        return 1;
    }

    shmid = shmget(share_key, 100,  IPC_CREAT | READ_WRITE);
    if(-1 == shmid)
    {
        perror("Error during shmget");
    }
    
    mem_block = shmat(shmid, NULL, 0);
    if((void *)-1 == mem_block)
    {
        perror("Error duting shmat");
    }

    for(; 100 > i; ++i)
    {
        printf("%c\n", mem_block[i]);
    }

    shmdt(mem_block);

    shmctl(shmid, IPC_RMID, NULL);

    printf("Printer: finished with block %d", shmid);

    return 0;
}