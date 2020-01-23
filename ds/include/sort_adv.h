#ifndef _SORTS_ADV
#define _SORTS_ADV
enum direction
{
	ASC, DSC
};

void HeapSort(int arr[], size_t n, enum direction direction);

void QuickSort(int arr[], size_t n, enum direction direction);

/* returns 1 if found, and updates index with the found index, 0 otherwise. */
int BinarySearch(const int arr[], int key, size_t n, size_t *index);

int JumpSearch(const int arr[], int key, size_t n, size_t *index);

#endif