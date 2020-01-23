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
*         SYSTEM PROGRAMMING: UID (TEST)               *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        BEN IZAKSON              * 
*******************************************************/
#include <stdio.h> /* printf */
#include <pthread.h> /* pthread_create */

#include "uid.h" 

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

#define NUM_OF_UIDS (10000)

void *CreateUID(void *args)
{
    unid_t *curr_uid = (unid_t *)args;

    *curr_uid = UIDCreate();

    return NULL;
}

void PrintUid(unid_t uid)
{
    printf("PID: %d\n", uid.pid);
    printf("TIME: %ld %ld\n", uid.time.tv_sec , uid.time.tv_usec);
    printf("COUNTER: %ld\n", uid.counter);
}

int TestCreate()
{
    unid_t test_uid1 = UIDCreate();
    unid_t test_uid2 = UIDCreate();

    test_uid2 = UIDCreate();

    PrintUid(test_uid1);
    printf("--------------\n");
    PrintUid(test_uid2);
    
    return 0;
}

int TestFlow()
{
    unid_t arr[NUM_OF_UIDS];
    pthread_t threads[NUM_OF_UIDS]; 
    int i = 0;
    int j = 0;
    
    for(; NUM_OF_UIDS > i; ++i)
    {
        pthread_create(threads + i, NULL, CreateUID, arr + i);
    }

    for(i = 0; NUM_OF_UIDS > i; ++i)
    {
        pthread_join(threads[i], NULL);   
    }

    for(i = 0; i < NUM_OF_UIDS; ++i)
    {
        for(j = 0; j < NUM_OF_UIDS; ++j)
        {
            if(j != i && UIDIsSame(arr[i], arr[j]))
            {
                return 1;
            }
        }
    }

    if(1 != UIDIsBad(bad_uid))
    {
        return 1;
    }

    return 0;
}

int main()
{
    RUN_TEST(TestCreate);
    RUN_TEST(TestFlow);

    return 0;
}
