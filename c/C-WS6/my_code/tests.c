#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "ws6.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"

#define UNUSED(x) (void)(x);

#define RUN_TEST(test_name) \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    }

int TestPow2()
{
    unsigned int test1_expected = 96;
    unsigned int test1 = Pow2(6, 4);

    unsigned int test2_expected = 8;
    unsigned int test2 = Pow2(32, -2);
    
    if(test1 != test1_expected || test2 != test2_expected) 
    {
        return 1;
    }

    return 0;
}

int TestIsPowOfTwoLoop()
{
    unsigned int test1_expected = 1;
    unsigned int test1 = IsPowOfTwoLoop(4);

    unsigned int test2_expected = 1;
    unsigned int test2 = IsPowOfTwoLoop(67108864);

    unsigned int test3_expected = 0;
    unsigned int test3 = IsPowOfTwoLoop(333);


    if(test1 != test1_expected || 
       test2 != test2_expected ||
       test3 != test3_expected) 
    {
        return 1;
    }

    return 0;
}

int TestIsPowOfTwo()
{
    unsigned int test1 = IsPowOfTwo(4); /* is */

    unsigned int test2 = IsPowOfTwo(64); /* is */

    unsigned int test3 = IsPowOfTwo(23); /* isn't */

/*    printf("1: %u 2: %u 3: %u\n", test1, test2, test3);*/

    if(1 != test1 ||
       1 != test2 ||
       0 != test3)
    {
        return 1;
    }

    return 0;
}

int TestAddOne()
{
    unsigned int test1_expected = 2;
    unsigned int test1 = AddOne(1);

    unsigned int test2_expected = 10;
    unsigned int test2 = AddOne(9);

    unsigned int test3_expected = 23;
    unsigned int test3 = AddOne(22);


    if(test1 != test1_expected || 
       test2 != test2_expected ||
       test3 != test3_expected) 
    {
        return 1;
    }

    return 0;
}

int TestTwoAndSix()
{
    uint_t test1_num = 162; /* 10100010 */
    uint_t test1 = AreSixAndTwoOn(test1_num);

    uint_t test2_num = 0; /* 00000000 */
    uint_t test2 = AreSixAndTwoOn(test2_num);

    uint_t test3_num = 1023; /* 000...1111111111 */
    uint_t test3 = AreSixAndTwoOn(test3_num);
    

    if(0 == test1 || 
       0 != test2 ||
       0 == test3) 
    {
        return 1;
    }
        
    return 0;
}

int TestTwoOrSix()
{
    uint_t test1_num = 162; /* 10100010 */
    uint_t test1 = AreSixAndTwoOn(test1_num);

    uint_t test2_num = 0; /* 00000000 */
    uint_t test2 = AreSixAndTwoOn(test2_num);

    uint_t test3_num = 32; /* 00100000 */
    uint_t test3 = AreSixAndTwoOn(test3_num);
    

    if(0 == test1 || 
       0 != test2 ||
       0 != test3) 
    {
        return 1;
    }
        
    return 0;
}

int SwapFiveThree()
{
    uint_t test1_num = 123; /* 01111011 */
    uint_t test1_expected = 111; /* 01101111 */
    uint_t test1 = SwapThreeFive(test1_num);

    uint_t test2_num = 25; /* 00011001 */
    uint_t test2_expected = 13; /* 00001101 */
    uint_t test2 = SwapThreeFive(test2_num);
      
    if(test1_expected != test1 || 
       test2_expected != test2) 
    {
        return 1;
    }
        
    return 0;
}

int TestCountSetBits()
{
    uint_t test1_num = 123; /* 01111011 */
    int test1_expected = 6; 
    int test1 = CountSetBitsLoop(test1_num);

    uint_t test2_num = 456; /* 111001000 */
    int test2_expected = 4; 
    int test2 = CountSetBitsLoop(test2_num);

    if(test1_expected != test1 || 
       test2_expected != test2) 
    {
        return 1;
    }
        
    return 0;
}

int TestCountSetBitsHamming()
{
    uint_t test1_num = 123; /* 01111011 */
    int test1_expected = 6; 
    int test1 = BitCountHamming(test1_num);

    uint_t test2_num = 456; /* 111001000 */
    int test2_expected = 4; 
    int test2 = BitCountHamming(test2_num);

    if(test1_expected != test1 || 
       test2_expected != test2) 
    {
        return 1;
    }
        
    return 0;
}

int TestFirstDevBySixteen()
{
    uint_t test1_num = 129; 
    int test1_expected = 128; 
    int test1 = FirstDevBySixteen(test1_num);

    uint_t test2_num = 70; 
    int test2_expected = 64; 
    int test2 = FirstDevBySixteen(test2_num);

    if(test1_expected != test1 || 
       test2_expected != test2) 
    {
        return 1;
    }
        
    return 0;
}

int TestSwap()
{
    uint_t test1_a = 2333;
    uint_t test1_b = 3222;

    uint_t test1_a_after = test1_b;
    uint_t test1_b_after = test1_a;

    SwapVars(&test1_a, &test1_b);

    if(test1_a != test1_a_after || test1_b != test1_b_after)
    {
        return 1;
    }
    
    return 0;
}

int main()
{
    RUN_TEST(TestPow2);
    RUN_TEST(TestIsPowOfTwoLoop);
    RUN_TEST(TestAddOne);
    RUN_TEST(TestTwoAndSix);
    RUN_TEST(TestTwoOrSix);
    RUN_TEST(SwapFiveThree);
    RUN_TEST(TestCountSetBits);
    RUN_TEST(TestFirstDevBySixteen);     
    RUN_TEST(TestSwap);
    RUN_TEST(TestIsPowOfTwo);
    RUN_TEST(TestCountSetBitsHamming);



    return 0;
}

