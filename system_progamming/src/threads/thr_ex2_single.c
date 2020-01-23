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
*   SYSTEM PROGRAMMING: THREADS EX 2 - SINGLE THREAD   *
*   CODE BY:            SASHA LIMAREV                  *
*  REVIEWED BY:        PROBABLY NOBODY                 * 
*******************************************************/
#include <assert.h> /* assert */
#include <stdio.h> /* printf */

typedef struct 
{
    size_t num;
    size_t sod;
} sum_info_t;


void *SumOfDevisors(void *args)
{
    size_t i = 1;
    sum_info_t *s_info = NULL;

    assert(args);

    s_info = (sum_info_t *)args;
    
    for(; i <= s_info->num; ++i)
    {
        if(0 == s_info->num % i)
        {
            s_info->sod += i;
        }
    
    }

    return NULL;
}


int main(int argc, char **argv)
{
    sum_info_t info = {0xFFF000, 0};

    SumOfDevisors(&info);

    printf("Num: %lu SOD: %lu\n", info.num ,info.sod);
    
    (void)argv;
    (void)argc;

    return 0;
}

/* 

Num: 16773120 SOD: 71556576

real    0m0.135s
user    0m0.135s
sys     0m0.000s

*/


