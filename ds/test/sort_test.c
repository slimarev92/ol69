#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */
#include <limits.h> /* USHRT_MAX */

#include "../include/sort.h" /*  scheduler header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"
#define PURPLE "\x1b[35m"
#define ORANGE "\x1b[33m"
#define CYAN "\x1b[34m"

#define ARR_SIZE (5000)

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    ++g_num_of_tests; \
    \
    if(0 == test_name()) \
    { \
        ++g_successful_tests; \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int g_num_of_tests = 0;
static int g_successful_tests = 0;

static size_t KeyToNumUShort(const void *data, const void *args)
{
    (void)args;
    
    return *(unsigned short *)data;
}

static int FirstIsLargerChar(const void *first, const void *second)
{
    int var1 = *(char *)first;
    int var2 = *(char *)second;

    return var1 <= var2 ? (var1 == var2 ? 0 : -1) : 1;
}

static int FirstIsLargerShort(const void *first, const void *second)
{
    int var1 = *(short *)first;
    int var2 = *(short *)second;

    return var1 <= var2 ? (var1 == var2 ? 0 : -1) : 1;
}

static int FirstIsLargerInt(const void *first, const void *second)
{
    int var1 = *(int *)first;
    int var2 = *(int *)second;

    return var1 <= var2 ? (var1 == var2 ? 0 : -1) : 1;;
}

static int FirstIsLargerLong(const void *first, const void *second)
{
    long var1 = *(long *)first;
    long var2 = *(long *)second;

    return var1 <= var2 ? (var1 == var2 ? 0 : -1) : 1;
}

int TestCountShort()
{
    size_t i = 0;
    unsigned short *test_arr = malloc(sizeof(unsigned short) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() % (ARR_SIZE);
    }
    
    /* sort array */
    CountingSort((void *)test_arr, ARR_SIZE,
                      sizeof(unsigned short), KeyToNumUShort, NULL, ARR_SIZE);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestBubbleChar()
{
    size_t i = 0;
    char *test_arr = malloc(sizeof(char) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() % 255;
    }

    /* sort array */
    BubbleSort((void *)test_arr, ARR_SIZE, sizeof(char), FirstIsLargerChar);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestBubbleShort()
{
    size_t i = 0;
    short *test_arr = malloc(sizeof(short) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() % ARR_SIZE;
        if(0 == i % 2)
        {
            test_arr[i] *= -1;
        }
    }
 
    /* sort array */
    BubbleSort((void *)test_arr, ARR_SIZE, sizeof(short), FirstIsLargerShort);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestBubbleInt()
{
    size_t i = 0;
    int *test_arr = malloc(sizeof(int) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() % ARR_SIZE;
    }

    /* sort array */
    BubbleSort((void *)test_arr, ARR_SIZE, sizeof(int), FirstIsLargerInt);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestBubbleLong()
{
    size_t i = 0;
    long *test_arr = malloc(sizeof(long) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand()  % ARR_SIZE;
    }

    /* sort array */
    BubbleSort((void *)test_arr, ARR_SIZE, sizeof(long), FirstIsLargerLong);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%ld) > index %ld (%ld)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestInsertionChar()
{
    size_t i = 0;
    char *test_arr = malloc(sizeof(char) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand()  % ARR_SIZE;
    }

    /* sort array */
    InsertionSort((void *)test_arr, ARR_SIZE, sizeof(char), FirstIsLargerChar);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestInsertionShort()
{
    size_t i = 0;
    short *test_arr = malloc(sizeof(short) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand()  % ARR_SIZE;
    }

    /* sort array */
    InsertionSort((void *)test_arr, ARR_SIZE, sizeof(short),
                                                     FirstIsLargerShort);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestInsertionInt()
{
    size_t i = 0;
    int *test_arr = malloc(sizeof(int) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand()  % ARR_SIZE;
    }

    /* sort array */
    InsertionSort((void *)test_arr, ARR_SIZE, sizeof(int), FirstIsLargerInt);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestInsertionLong()
{
    size_t i = 0;
    long *test_arr = malloc(sizeof(long) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand()  % ARR_SIZE;
    }

    /* sort array */
    InsertionSort((void *)test_arr, ARR_SIZE, sizeof(long), FirstIsLargerInt);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%ld) > index %ld (%ld)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestSelectionChar()
{
    size_t i = 0;
    char *test_arr = malloc(sizeof(char) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand()  % ARR_SIZE;
    }

    /* sort array */
    SelectionSort((void *)test_arr, ARR_SIZE, sizeof(char), 
                                                          FirstIsLargerChar);
    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestSelectionShort()
{
    size_t i = 0;
    short *test_arr = malloc(sizeof(short) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() % ARR_SIZE;
    }

    /* sort array */
    SelectionSort((void *)test_arr, ARR_SIZE, sizeof(short), 
                                                          FirstIsLargerShort);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestSelectionInt()
{
    size_t i = 0;
    int *test_arr = malloc(sizeof(int) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() / 100000;
    }

    /* sort array */
    SelectionSort((void *)test_arr, ARR_SIZE, sizeof(int), FirstIsLargerInt);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {
            printf("    %sLine %d - Index %ld (%d) > index %ld (%d)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestSelectionLong()
{
    size_t i = 0;
    long *test_arr = malloc(sizeof(long) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() / 100000;
    }

    /* sort array */
    SelectionSort((void *)test_arr, ARR_SIZE, sizeof(long), FirstIsLargerInt);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {           
            printf("    %sLine %d - Index %ld (%ld) > index %ld (%ld)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestTest() /* checks that tests work */
{
    size_t i = 0;
    long *test_arr = malloc(sizeof(long) * ARR_SIZE);
    if(NULL == test_arr)
    {
        return 0;
    }

    /* intialize array with random numbers */
    for(; i < ARR_SIZE; ++i) 
    {
        test_arr[i] = rand() % ARR_SIZE;
        if(0 == i % 2)
        {
            test_arr[i] *= -1;
        }
    }

    /* sort array */
    qsort((void *)test_arr, ARR_SIZE, sizeof(long), FirstIsLargerInt);

    /* check if array is properly sorted */
    for(i = 0; i < ARR_SIZE - 1; ++i)
    {
        if(test_arr[i] > test_arr[i + 1])
        {            
            printf("    %sLine %d - Index %ld (%ld) > index %ld (%ld)\n",
                     RED, __LINE__ - 3, i, test_arr[i], i + 1, test_arr[i + 1]);
            free(test_arr);
            return 1;
        }
    }

    free(test_arr);
    return 0;
}

int TestBubble()
{
    return TestBubbleChar() | TestBubbleShort() | 
            TestBubbleInt() | TestBubbleLong();
}

int TestInsertion()
{
    return TestInsertionChar() | TestInsertionShort() | 
            TestInsertionInt() | TestInsertionLong();
}

int TestCount()
{
    return TestCountShort(); 
    
}

int main()
{
    srand((unsigned int)time(NULL));
 
/*    RUN_TEST(TestTest);*/
/*    RUN_TEST(TestBubble);*/
/*    RUN_TEST(TestInsertion);*/
/*    RUN_TEST(TestSelectionShort);*/
    RUN_TEST(TestCount);
/*    RUN_TEST(TestRadixShort);*/

    return 0;
}
