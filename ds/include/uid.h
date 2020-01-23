#ifndef UID_H
#define UID_H

#include <sys/time.h>
#include <sys/types.h>

typedef struct unid
{
	pid_t pid;
	struct timeval time;
	size_t counter;
} unid_t;

extern const unid_t bad_uid;

unid_t UIDCreate();

int UIDIsSame(unid_t uid1, unid_t uid2);

int UIDIsBad(unid_t uid);

#endif
