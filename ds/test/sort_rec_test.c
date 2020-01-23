#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */
#include <limits.h> /* USHRT_MAX */

#include "sort_rec.h" /*  scheduler header file */

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

int TestMerge()
{    
    int nums[] = {  13,  62,  73,  94,  38,
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

    size_t arr_size = 100;
    size_t i = 0;

/*    printf("\tBefore sorting:\n");

    for(i = 0; arr_size > i; ++i)
    {
        printf("\tnums[%ld] = %d\n", i, nums[i]);
    }

    printf("\t-----------------------------\n");*/

    /* merge sort array */
    MergeSort(nums, arr_size);

   /* printf("\tAfter sorting:\n");*/

    /* check if array is sorted */
    for(i = 0; i < arr_size - 1; ++i)
    {
        if(nums[i] > nums[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, nums[i], i + 1, nums[i + 1]);
            return 1;
        }

      /*  printf("\tnums[%ld] = %d\n", i, nums[i]);*/
    }

    return 0;
}

int main()
{
    /* srand((unsigned int)time(NULL)); */

    RUN_TEST(TestMerge);

    return 0;
}
