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
*         SYSTEM PROGRAMMING: UID                      *
*         CODE BY:            SASHA LIMAREV            *
*         REVIEWED BY:        BEN IZAKSON              * 
*******************************************************/
#include <unistd.h> /* getpid */

#include "uid.h" /* uid library header */

const unid_t bad_uid;
static size_t uid_counter = 0; 

unid_t UIDCreate()
{
    unid_t new_unid;
    
    new_unid.pid = getpid();
    new_unid.counter = __sync_fetch_and_add(&uid_counter, 1);
    gettimeofday(&new_unid.time, NULL);

    return new_unid;
}

int UIDIsSame(unid_t uid1, unid_t uid2)
{
    int test1 = uid1.pid == uid2.pid;
    int test2 = uid1.time.tv_sec == uid2.time.tv_sec;
    int test3 = uid1.time.tv_usec == uid2.time.tv_usec;
    int test4 = uid1.counter == uid2.counter;

    return test1 && test2 && test3 && test4;
}

int UIDIsBad(unid_t uid)
{
    return UIDIsSame(uid, bad_uid);
}
