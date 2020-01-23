/********************************
* DS:          TRIE (HEADER)    *
* CODE BY:     SASHA LIMAREV    *   
* REVIEWED BY: ROI KRAMER       *                          
********************************/
#ifndef TRIE_SLIMAREV
#define TRIE_SLIMAREV

typedef struct trie trie_t;

enum trie_status
{
    ADDED, /* value didn't exist in the tree and was added */
    ADDED_EXIST, /* value existed in the tree, closest value was added */
    NADDED_EXIST, /* value existed in the tree, no value was added */
    FULL, /* tree below node is full */
    MEM_FAIL, /* memory fail */
    REMOVED, /* node was removed */
    NREMOVED /* node was not removed */
};

/*returns point to trie with n bits, trie can't be NULL */
trie_t *TrieCreate(size_t n_bits);

/* destroy trie and free its memory */
void TrieDestroy(trie_t *trie);

/* returns ADDED if value didn't exist in trie and was added to it, otherwise 
returns NADDED_EXIST or FULL*/
enum trie_status TrieInsertExact(trie_t *trie, const void *value);

/* returns ADDED if value didn't exist in trie and was added to it, otherwise 
returns ADDED_EXIST or FULL*/
enum trie_status TrieInsertClose(trie_t *trie, const void *value, void *res);

/* returns the number of values in the trie */
size_t TrieCount(trie_t *trie);

/* removes value from trie, returns REMOVED if the value is removed,
NREMOVED otherwise */
enum trie_status TrieRemove(trie_t *trie, const void *value);

#endif /* TRIE_SLIMAREV */