#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */

#include "sort_adv.h" /*  scheduler header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"
#define PURPLE "\x1b[35m"
#define ORANGE "\x1b[33m"
#define CYAN "\x1b[34m"

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int TestQuickSortDSC()
{
    size_t i = 0;
    size_t arr_size = 100;
    static int nums[] = {   13,  22,  73,  94,  38,
                            50,  78,  43,  28,  57,
                            24,  78,  69,  71,  5,
                            36,  88,  4,   1,   59,
                            69,  69,  3,   16,  23,
                            43,  95,  1,   78,  16,
                            54,  27,  73,  52,  44,
                            31,  55,  39,  1,   62,
                            80,  79,  45,  29,  8,
                            32,  49,  42,  64,  31,
                            96,  71,  89,  85,  52,
                            72,  91,  58,  49,  77,
                            58,  90,  2,   68,  78,
                            100, 54,  40,  40,  13,
                            15,  19,  7,   69,  38,
                            82,  83,  2,   25,  69,
                            24,  98,  71,  25,  38,
                            30,  76,  36,  78,  47,
                            26,  30,  74,  32,  64,
                            64,  82,  98,  90,  80
                        };

    QuickSort(nums, arr_size, DSC);

    for(i = 0; i < arr_size - 1; ++i)   
    {
        if(nums[i] < nums[i + 1])
        {
            printf("\t%sLine %d - arr[%ld] - %d < arr[%ld] - %d\n",
                 RED, __LINE__ - 4, i, nums[i], i + 1, nums[i + 1]);
        return 1;
        } 
    }

    return 0;
}

static int TestQuickSortASC()
{
    size_t i = 0;
    size_t arr_size = 100;
    static int nums[] = {   13,  22,  73,  94,  38,
                            50,  78,  43,  28,  57,
                            24,  78,  69,  71,  5,
                            36,  88,  4,   1,   59,
                            69,  69,  3,   16,  23,
                            43,  95,  1,   78,  16,
                            54,  27,  73,  52,  44,
                            31,  55,  39,  1,   62,
                            80,  79,  45,  29,  8,
                            32,  49,  42,  64,  31,
                            96,  71,  89,  85,  52,
                            72,  91,  58,  49,  77,
                            58,  90,  2,   68,  78,
                            100, 54,  40,  40,  13,
                            15,  19,  7,   69,  38,
                            82,  83,  2,   25,  69,
                            24,  98,  71,  25,  38,
                            30,  76,  36,  78,  47,
                            26,  30,  74,  32,  64,
                            64,  82,  98,  90,  80
                        };

    QuickSort(nums, arr_size, ASC);

    for(i = 0; i < arr_size - 1; ++i)   
    {
        if(nums[i] > nums[i + 1])
        {
            printf("\t%sLine %d - arr[%ld] - %d > arr[%ld] - %d\n",
                 RED, __LINE__ - 4, i, nums[i], i + 1, nums[i + 1]);
        return 1;
        } 
    }

    return 0;
}

static int TestQuickSortBigDSC()
{
    size_t i = 0;
    size_t arr_size = 100000;
    int *nums = (int *)malloc(sizeof(int) * arr_size);

    /* fill array with random nums */
    for(; i < arr_size; ++i)
    {
        nums[i] = rand() % 10000000;
    }

    /* sort array in ascending order */
    QuickSort(nums, arr_size, DSC);

    /* check if array is sorted */
    for(i = 0; i < arr_size - 1; ++i)   
    {
        if(nums[i] < nums[i + 1])
        {
            printf("\t%sLine %d - arr[%ld] - %d < arr[%ld] - %d\n",
                 RED, __LINE__ - 4, i, nums[i], i + 1, nums[i + 1]);
        free(nums);
        return 1;
        } 
    }

    free(nums);
    return 0;
}

static int TestQuickSortBigASC()
{
    size_t i = 0;
    size_t arr_size = 100000;
    int *nums = (int *)malloc(sizeof(int) * arr_size);

    /* fill array with random nums */
    for(; i < arr_size; ++i)
    {
        nums[i] = rand() % 10000000;
    }

    /* sort array in ascending order */
    QuickSort(nums, arr_size, ASC);

    /* check if array is sorted */
    for(i = 0; i < arr_size - 1; ++i)   
    {
        if(nums[i] > nums[i + 1])
        {
            printf("\t%sLine %d - arr[%ld] - %d > arr[%ld] - %d\n",
                 RED, __LINE__ - 4, i, nums[i], i + 1, nums[i + 1]);
        free(nums);
        return 1;
        } 
    }

    free(nums);
    return 0;
}

static int TestHeapSortDSC()
{
    size_t i = 0;
    size_t arr_size = 100;
    static int nums[] = {   13,  62,  73,  94,  38,
                            50,  78,  43,  28,  57,
                            24,  78,  69,  71,  5,
                            36,  88,  4,   1,   59,
                            69,  69,  3,   16,  23,
                            43,  95,  1,   78,  16,
                            54,  27,  73,  52,  44,
                            31,  55,  39,  1,   62,
                            80,  79,  45,  29,  8,
                            32,  49,  42,  64,  31,
                            96,  71,  89,  85,  52,
                            72,  91,  58,  49,  77,
                            58,  90,  2,   68,  78,
                            100, 54,  40,  40,  13,
                            15,  19,  7,   69,  38,
                            82,  83,  2,   25,  69,
                            24,  98,  71,  25,  38,
                            30,  76,  36,  78,  47,
                            26,  30,  74,  32,  64,
                            64,  82,  98,  90,  80
                        };

    HeapSort(nums, arr_size, DSC);

    for(i = 0; i < arr_size - 1; ++i)   
    {
        if(nums[i] < nums[i + 1])
        {
            printf("\t%sLine %d - arr[%ld] - %d < arr[%ld] - %d\n",
                 RED, __LINE__ - 4, i, nums[i], i + 1, nums[i + 1]);
        return 1;
        } 
    }

    return 0;
}

static int TestHeapSortASC()
{
    size_t i = 0;
    size_t arr_size = 100;
    static int nums[] = {   13,  62,  73,  94,  38,
                            50,  78,  43,  28,  57,
                            24,  78,  69,  71,  5,
                            36,  88,  4,   1,   59,
                            69,  69,  3,   16,  23,
                            43,  95,  1,   78,  16,
                            54,  27,  73,  52,  44,
                            31,  55,  39,  1,   62,
                            80,  79,  45,  29,  8,
                            32,  49,  42,  64,  31,
                            96,  71,  89,  85,  52,
                            72,  91,  58,  49,  77,
                            58,  90,  2,   68,  78,
                            100, 54,  40,  40,  13,
                            15,  19,  7,   69,  38,
                            82,  83,  2,   25,  69,
                            24,  98,  71,  25,  38,
                            30,  76,  36,  78,  47,
                            26,  30,  74,  32,  64,
                            64,  82,  98,  90,  80
                        };

    HeapSort(nums, arr_size, ASC);

    for(i = 0; i < arr_size - 1; ++i)   
    {
        if(nums[i] > nums[i + 1])
        {
            printf("\t%sLine %d - arr[%ld] - %d > arr[%ld] - %d\n",
                 RED, __LINE__ - 4, i, nums[i], i + 1, nums[i + 1]);
        return 1;
        } 
    }

    return 0;
}

static int TestBinSearch()
{
    size_t i = 0;
    size_t arr_size = 20;
    static size_t indices[] = {0, 0, 0, 0, 0};
    static size_t ex_indices[] = {1, 3, 4, 17, 19};
    static int keys[] = {8, 23, 30, 91, 96};
    static int status[] = {0, 0, 0, 0, 0, 0, 0};
    int big_key = 999;
    int small_key = -10;

    static int nums[] = {   7,  8,  13, 23, 30,
                            32, 36, 37, 40, 43,
                            48, 51, 55, 68, 72,
                            85, 88, 91, 93, 96
                        };

    /* find indices of nums in array */
    for(; i < 5; ++i)
    {
        status[i] =  BinarySearch(nums, keys[i], arr_size, &indices[i]);;
    }

    /* check if the returned indices are correct */
    for(i = 0; i < 5; ++i)
    {
        if(indices[i] != ex_indices[i] || 1 != status[i])
        {
            printf("\t%sLine %d - Key %d Expected index %ld, got index %ld\n",
                 RED, __LINE__ - 4, keys[i], ex_indices[i], indices[i]);
        return 1;
        } 
    }

    /* try to find number not in array */
    if(1 == BinarySearch(nums, big_key, arr_size, &indices[0]))
    {
        printf("\t%sLine %d - Search for %d that doesnt exist returned 1\n",
                 RED, __LINE__ - 4, big_key);
        return 1;
    }

    /* try to find number not in array */
    if(1 == BinarySearch(nums, small_key, arr_size, &indices[0]))
    {
        printf("\t%sLine %d - Search for %d that doesnt exist returned 1\n",
                 RED, __LINE__ - 4, small_key);
        return 1;
    }

    return 0;
}

static int TestJumpSearch()
{
    size_t i = 0;
    size_t arr_size = 20;
    static size_t indices[] = {100, 100, 100, 100, 100};
    static size_t ex_indices[] = {0, 3, 4, 17, 19};
    static int keys[] = {7, 23, 30, 91, 96};
    static int status[] = {0, 0, 0, 0, 0, 0, 0};
    int big_key = 999;
    int small_key = -10;

    static int nums[] = {   7,  8,  13, 23, 30,
                            32, 36, 37, 40, 43,
                            48, 51, 55, 68, 72,
                            85, 88, 91, 93, 96
                        };

    /* find indices of nums in array */
    for(i = 0; i < 5; ++i)
    {
        status[i] =  JumpSearch(nums, keys[i], arr_size, &indices[i]);;
    }

    /* check if the returned indices are correct */
    for(i = 0; i < 5; ++i)
    {
        if(indices[i] != ex_indices[i] || 1 != status[i])
        {
            printf("\t%sLine %d - Key %d Expected index %ld, got index %ld\n",
                 RED, __LINE__ - 4, keys[i], ex_indices[i], indices[i]);
        return 1;
        } 
    }

    /* try to find number not in array */
    if(1 == JumpSearch(nums, big_key, arr_size, &indices[0]))
    {
        printf("\t%sLine %d - Search for %d that doesnt exist returned 1\n",
                 RED, __LINE__ - 4, big_key);
        return 1;
    }

    /* try to find number not in array */
    if(1 == JumpSearch(nums, small_key, arr_size, &indices[0]))
    {
        printf("\t%sLine %d - Search for %d that doesnt exist returned 1\n",
                 RED, __LINE__ - 4, small_key);
        return 1;
    }

    return 0;
}

int main()
{
    srand((unsigned int)time(NULL)); 

    RUN_TEST(TestHeapSortASC);
    RUN_TEST(TestHeapSortDSC);
    RUN_TEST(TestQuickSortASC);
    RUN_TEST(TestQuickSortDSC);
    RUN_TEST(TestQuickSortBigASC);
    RUN_TEST(TestQuickSortBigDSC);
    RUN_TEST(TestBinSearch);
    RUN_TEST(TestJumpSearch);

    return 0;
}
