#include <sys/ipc.h> /* shmget */
#include <sys/shm.h> /* shmget */
#include <stdio.h> /* perror */

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
        mem_block[i] = (i % 11) + '0';
    }

    shmdt(mem_block);

    printf("Filler: finished with block %d", shmid);

    return 0;
}