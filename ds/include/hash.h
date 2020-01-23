/******************************************
*                _____  __     _   ___    *
*               (  _  )(  )   / ) / _ \   *
*    |\---/|     )(_)(  )(__ / _ \\_  /   *
*    | ,_, |    (_____)(____)\___/ (_/    *
*     \_`_/-..----.                       *
*  ___/ `   ' ,""+ \                      *
* (__...'   __\    |`.___.';              *
*  (_,...'(_,.`__)/'.....+                *
*                                         *        
*            Exersice: Hash Table         *
*******************************************/

#ifndef HASH_H
#define HASH_H

typedef struct hash hash_t;

/* Create the hash table.
Return value: a pointer to the hash table.
Compare != NULL
key != NULL
range > 0 */
hash_t *HashCreate(int (*Compare)(const void *data, const void *key),
					size_t (*HashFunction)(void *key),
					size_t range);

/* Destroy the table. 
table != NULL */
void HashDestroy(hash_t *table);

/* Insert data to the table.
Return value: 1 - for successful insertion, 0 - for failure 
table != NULL 
data != NULL */
int HashInsert(hash_t *table, void *data);

/* Remove the key from the list.
table != NULL
key != NULL */
void HashRemove(hash_t *table, void *key);

/* Find the key in the table.
Return value: data if found, NULL else.
table != NULL.
key != NULL; */
void *HashFind(const hash_t *table, void *key);

/* Calculate the sum of the elemets in the table.
Return vlue: returns number of inserted data 
table != NULL */
size_t HashSize(const hash_t *table);

/* Check if the table is empty.
Return value: 1 if empty, 0 else 
table != NULL */
int HashIsEmpty(const hash_t *table);

/* Perform the action function on each element in the table.
Return value: 1 if OK for all, 0 else 
table != NULL
act_f != NULL */
int HashForEach(hash_t *table, 
				int (*Action)(void *data, const void *params),
				const void *params);

/* Calculate the load on the table.
Return value: HashSize / range
table != NULL */
double HashLoad(const hash_t *table);

/* Calculate standart error.
Rerurn value: STD / HashSize
STD = root of: (sum of every element - avrage) / HashSize.
table != NULL */
double HashSD(const hash_t *table);


/* for testing, insert all of the dictionary(/usr/share/dict) and test finding */

#endif