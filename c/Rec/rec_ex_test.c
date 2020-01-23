#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "recursion.h" /* rec header file */

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
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

void PrintSLLData(node_t *node)
{
    printf("\tcurr data = %ld\n", (size_t)(node->data));

    if(NULL != node->next)
    {
        PrintSLLData(node->next);
    }
}

void PrintStack(stack_t *stack)
{
    int temp = *(int *)STACKPeek(stack);

    printf("\t%d\n", temp);

    if(1 < STACKSize(stack))
    {
        STACKPop(stack);
        PrintStack(stack);
        STACKPush(stack, (void *)&temp);
    }
}

int TestFibIter()
{
    long expected_fibs[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
    long i = 0;
    long curr_fib = 0;

    for(; 10 > i; ++i)
    {
        curr_fib = IterFibonacci(i + 1);

        if(expected_fibs[i] != curr_fib)
        {
            printf("    %sLine %d - Expected: %ld, Actual: %ld (i=%ld)\n",
                   RED, __LINE__ - 3, expected_fibs[i], curr_fib, i);
            return 1;
        }
    }

    return 0;
}

int TestFibRec()
{
    long expected_fibs[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
    long i = 0;
    long curr_fib = 0;

    for(; 10 > i; ++i)
    {
        curr_fib = RecurFibonacci(i + 1);

        if(expected_fibs[i] != curr_fib)
        {
            printf("    %sLine %d - Expected: %ld, Actual: %ld (i=%ld)\n",
                   RED, __LINE__ - 3, expected_fibs[i], curr_fib, i);
            return 1;
        }
    }

    return 0;
}

int TestStrLen()
{
    char *strs[] = {"sasha", "my name is sasha", ""};
    size_t expected_lens[] = {5, 16, 0};
    size_t i = 0;
    size_t curr_size = 0;

    for(; 3 > i; ++i)
    {
        curr_size = RecurStrlen(strs[i]);

        if(expected_lens[i] != curr_size)
        {
            printf("    %sLine %d - Expected: %ld, Actual: %ld (i=%ld)\n",
                   RED, __LINE__ - 3, expected_lens[i], curr_size, i);
            return 1;
        }
    }

    return 0;
}

int TestStrCnp()
{
    char *strs1[] = {"111", "110", "111", "1113", "11"};
    char *strs2[] = {"111", "111", "110", "111", "111"};
    int expected_cmps[] = {0, -1, 1, 3, -1};
    size_t i = 0;
    int curr_cmp = 0;

    for(; 3 > i; ++i)
    {
        curr_cmp = RecurStrcmp(strs1[i], strs2[i]);

        if(expected_cmps[i] != curr_cmp)
        {
            printf("    %sLine %d - Expected: %d, Actual: %d (i=%ld)\n",
                   RED, __LINE__ - 3, expected_cmps[i], curr_cmp, i);
            return 1;
        }
    }

    return 0;
}

int TestCpy()
{
    char *test_str = "I am Sam. I am Sam. Sam-I-Am.";   
    char *dest = calloc(sizeof(char), 100);
    if(NULL == dest)
    {
        free(dest);
    }

    RecurStrcpy(dest, test_str);

    if(0 != RecurStrcmp(dest, test_str))
    {
        free(dest);
        printf("\tsrc: %s\n", test_str);
        printf("\tdest: %s\n", dest);
        return 1;
    }

    free(dest);
    return 0;
}

int TestFlip()
{
    size_t i = 0;
    size_t expected[] = {4 ,3, 2, 1};
    
    static node_t node_d = {(void *)4, NULL};
    static node_t node_c = {(void *)3, &node_d};
    static node_t node_b = {(void *)2, &node_c};
    static node_t node_a = {(void *)1, &node_b};
    node_t *curr = &node_a;

    printf("\tBefore Flip: \n");
    PrintSLLData(&node_a);
    printf("\n");
    printf("\tAfter Flip: \n");

    curr = RecurFlip(&node_a);

    PrintSLLData(curr);

    for(; i < 4; ++i)
    {
        if((size_t)curr->data != expected[i])
        {
            return 1;
        }

        curr = curr->next;
    }

    return 0;
}

int TestSort()
{
    size_t i = 0;
    int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    stack_t *test_stack = STACKCreate(10, sizeof(int));
  
    for(i = 1; i <= 10; ++i)
    {
        STACKPush(test_stack, (void *)&i);
    }

    printf("\tBefore sort:\n");
    PrintStack(test_stack);
    printf("\n");

    RecurStackSort(test_stack); 
    
    printf("\tAfter sort:\n");
    PrintStack(test_stack);

    for(; 10 >= i; ++i)
    {
        if(expected[i - 1] != *(int *)STACKPeek(test_stack))
        {
            STACKDestroy(test_stack);
            return 1;
        }

        STACKPop(test_stack);
    }

    STACKDestroy(test_stack);
    return 0;
}

int main()
{
    RUN_TEST(TestFibIter);
    RUN_TEST(TestFibRec);
    RUN_TEST(TestStrLen);
    RUN_TEST(TestStrCnp);
    RUN_TEST(TestCpy);
    RUN_TEST(TestFlip);
    RUN_TEST(TestSort);

    return 0;
}
