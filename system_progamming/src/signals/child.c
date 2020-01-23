/**********************************************************
* SYSTEM PROGRAMMING: SIMPLE WATCHDOG (CHILD)             *
* CODE BY:            SASHA LIMAREV                       *   
* REVIEWED BY:        MISHEL LIBERMAN                     *  
***********************************************************/
#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */

int main()
{
    printf("I'm a child (PID %d), going to sleep for five seconds\n", getpid());

    sleep(5);

    return 0;
}