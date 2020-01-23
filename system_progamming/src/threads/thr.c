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
*         SYSTEM PROGRAMMING: PRODUCER CONSUMER 1      *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        YOAV KLEIN               * 
*******************************************************/
#include <pthread.h> /* pthread_create */
#include <stdio.h> /* printf */

#define ARR_SIZE (1000)
#define MAX_NUM (100)

static int g_arr[ARR_SIZE] = {0};
static volatile size_t g_done_flag = 0;

void *FillArr(void *args)
{
    size_t i = 0;
    size_t num = 1;

    (void)args;

    while(MAX_NUM >= num)
    {
        while(0 != g_done_flag)
            ;
            
        for(i = 0; i < ARR_SIZE; ++i)
        {
            g_arr[i] = num;
        }

        ++num;
        g_done_flag = 1;
    }

    return NULL;
}

int main()
{
    pthread_t producer;
    size_t sum = 0;
    size_t i = 0;
    size_t num = 1;

    pthread_create(&producer, NULL, FillArr, NULL);

    while(MAX_NUM >= num)
    {    
        while(0 == g_done_flag)
            ;
        
        for(i = 0, sum = 0; i < ARR_SIZE; ++i)
        {
            sum += g_arr[i];
        }

        printf("num is %3.lu, sum is %lu\n", num, sum);

        ++num;
        g_done_flag = 0; 
    }

    pthread_join(producer, NULL);

    return 0;
}