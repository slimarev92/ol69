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
*     PROJECTS: WATCH DOG (TEST)                       *
*     CODE BY:            SASHA LIMAREV                *
*     REVIEWED BY:        ANDREI PLAKHOTA              *
*******************************************************/
#include <unistd.h> /* sleep */
#include <stdio.h> /* printf */

#include "watchdog.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define RESET_COLOR "\x1b[0m"

int main(int argc, char **argv)
{
    int time_to_sleep = 35;

    WDStart(argc, argv, 23);
    
    printf("%sApp: is up!\n", GREEN);

    while(0 < time_to_sleep)
    {
        time_to_sleep = sleep(time_to_sleep);
    }

    printf("%sApp: Telling wd to stop\n", RED);

    WDStop();

    return 0;
}