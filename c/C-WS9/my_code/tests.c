#include <stdio.h> /* printf */
#include <stdlib.h> /* system */
#include <string.h> /* memcmp */
#include "ws9.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"

#define WORD (size_t) 8

#define RUN_TEST(test_name) \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    }

void test_cpy()
{
    int test = 0;
    int i = 0;

    long abc[] = {10, 20, 30, 40, -100, 500};
    int times = sizeof(abc);
    int offset = 3;
    long *abc_cpy = (long *) malloc(sizeof(long) * 10);
    char *abc_offset = ((char *) abc_cpy + offset);
    if(NULL == abc_cpy)
    {
        exit(1);
    }

    MemCpy(abc_offset, abc, times);    

    test = memcmp(abc, abc_offset, times);
    
    printf("TEST: %d TIMES: %d\n",test, times);

    for(; i < 6; ++i)
    {
      printf("%ld %ld\n", (long) abc[i]  , *((long *)abc_offset + i));
    }

    free(abc_cpy); 
}

int TestItoaAny()
{
    char *block = (char *) malloc(sizeof(char) * 20);

    int test1_num = 126;
    int test1_base = 5;
    char *test1_expected = "1001";
    char *test1 = block; 
    int test1_res = 23;

    int test2_num = -126;
    int test2_base = 10;
    char *test2_expected = "-126";
    char *test2 = block; 
    int test2_res = 23;

    int test3_num = 23;
    int test3_base = 2;
    char *test3_expected = "10111";
    char *test3 = block; 
    int test3_res = 23;
  
    int test4_num = 2567;
    int test4_base = 36;
    char *test4_expected = "1zb";
    char *test4 = block; 
    int test4_res = 23;

    ItoaAny(test1_num, test1, test1_base);
    test1_res = strcmp(test1_expected, test1);

    ItoaAny(test2_num, test2, test2_base);
    test2_res = strcmp(test2_expected, test2);

    ItoaAny(test3_num, test3, test3_base);
    test3_res = strcmp(test3_expected, test3);

    ItoaAny(test4_num, test4, test4_base);
    test4_res = strcmp(test4_expected, test4);

    if(0 != test1_res || 0 != test2_res ||
       0 != test3_res || 0 != test4_res)
    {
        free(block);
        return 1;
    }
    
    free(block);
    return 0;
}

int TestAtoiAny()
{
    int test1_expected = 2567;
    int test1 = AtoiAny("1zb", 36);

    int test2_expected = 23;
    int test2 = AtoiAny("10111", 2);
    
    int test3_expected = -126;
    int test3 = AtoiAny("-126", 10);

    int test4_expected = 126;
    int test4 = AtoiAny("1001", 5);

    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }   
    
    return 0;
}

void TestMove3()
{
    int i = 0;
    int n_times = 4;
    int offset = 5;

    long nums[] = {10, 20, 30, 40};
    long *num_cpy = (long *) malloc(sizeof(long) * 10);
    long *num_cpy_offset = (long *) ((char *) num_cpy + offset);   

    MemMove(num_cpy_offset, nums, sizeof(long) * n_times);
    MemMove(num_cpy_offset, num_cpy_offset + 2, sizeof(long) * 2);


    for(; i < n_times; ++i)
    {
        printf("%ld %ld\n", nums[i], num_cpy_offset[i]);
    }

    free(num_cpy);
}

void TestMove4()
{
    int offset = 7;

    char* nums = "12345678-87654321-00000000";
    char *expected = "2345678-887654321-00000000";
    char *num_cpy = (char *) calloc(sizeof(char) , 40);
    char *num_cpy_offset = num_cpy + offset;   
    int n_times = strlen(nums);

    MemMove(num_cpy_offset, nums, sizeof(char) * n_times);
    MemMove(num_cpy_offset, num_cpy_offset + 1, sizeof(char) * 9);

    printf("%s\n%s\n", expected, num_cpy_offset);

    free(num_cpy);
}

int main()
{
    RUN_TEST(TestItoaAny);
/*    RUN_TEST(TestAtoiAny);*/

/*    TestMove3();*/
/*    TestMove4();*/

    return 0;
}

