#ifndef RECURSION_H
#define RECURSION_H

#include <stddef.h> /* size_t */
#include "../../ds/include/stack.h" /* stack header file */

typedef struct node
{
	void *data;
	struct node *next;
} node_t;

long IterFibonacci(long n);

long RecurFibonacci(long n);

size_t RecurStrlen(const char *s);

int RecurStrcmp(const char *s1, const char *s2);

char *RecurStrcpy(char *dest, const char *src);

node_t *RecurFlip(node_t *head);

/*
	the stack contains only integers.
	the min number should be at the top of the stack.
*/
void RecurStackSort(stack_t *stack);

/* extra */

char *RecurStrcat(char *dest, const char *src);

char *RecurStrstr(const char *haystack, const char *needle);


#endif          /* recursion  */