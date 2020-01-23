/*
Compare < 0 *data1 goes before *data2
Compare == 0 *data1 is equivalent to *data2
Compare > 0 *data1 goes after *data2
*/
typedef int (*cmp_f)(const void *data1, const void *data2);

typedef size_t (*key_to_num)(const void *data, const void *args);

/* base must not be NULL, num of members must be greater than 2 */
/* element size must be greater than 0, compare must not be NULL */
void BubbleSort(void *base, size_t num_of_members, size_t element_size, cmp_f cmp);

/* base must not be NULL, num of members must be greater than 2 */
/* element size must be greater than 0, compare must not be NULL */
void InsertionSort(void *base, size_t num_of_members, size_t element_size, cmp_f cmp);

/* base must not be NULL, num of members must be greater than 2 */
/* element size must be greater than 0, compare must not be NULL */
void SelectionSort(void *base, size_t num_of_members, size_t element_size, cmp_f cmp);

/* base must not be NULL, num of members must be greater than 2 */
/* element size must be greater than 0, key_to_num must not be NULL */
/* range is exclusive and must be greater than 1 */
/* for ex. array size of 9 range range will be 9 */
/* returns -1 on failure and 0 on success */
int CountingSort(void *base, size_t num_of_members, size_t element_size,
					key_to_num ktn, const void *args, size_t range);
					
/* base must not be NULL, num of members must be greater than 2  */
/* element size must be greater than 0, key_to_num must not be NULL */
/* range is exclusive and must be greater than 1 */
/* for ex. array size of 9 range range will be 9 */
/* returns -1 on failure and 0 on success */
int RadixSort(void *base, size_t num_of_members, size_t element_size,
					key_to_num ktn, const void *args, size_t num_of_bytes);
