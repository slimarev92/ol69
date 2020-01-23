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
#include <omp.h> /* prgama omp */

typedef struct 
{
    size_t num;
    size_t sod;
} sum_info_t;

int main()
{
    size_t i = 0;
    sum_info_t info = {0xFFF000, 0};
    
    #pragma omp parallel for
    for(i = 1; i <= info.num; ++i)
    {
        if(0 == info.num % i)
        {
            info.sod += i;
        }   
    }

    printf("Num: %lu SOD: %lu\n", info.num ,info.sod);

    return 0;
}

/*

Num: 16773120 SOD: 71556576

real    0m0.050s
user    0m0.184s
sys 0m0.000s

*/

