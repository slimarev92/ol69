#include <stdio.h> /* printf */
#include "../include/barr.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"

#define WORD (size_t) 8

#define RUN_TEST(test_name) \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s", RESET_COLOR);

extern int g_count_lut_counter;
extern int g_mirror_lut_counter;

/*static void DisplayBits(size_t var) 
{
    size_t mask = 1;
    size_t i = 0;

    size_t var_size = sizeof(size_t);
   
    mask = mask << ((var_size * 8) - 1);

    for(; i < var_size * 8; ++i)
    {
        size_t curr_bit = mask & var;

        if(0 == (i % 8) && 0 < i)
        {
            printf("-");
        }
        
        if(0 != curr_bit)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }

        mask = mask >> 1;
    }

    printf("\n");
}    */

int TestSet()
{
    size_t test1_expected = 0;
    size_t test1 = BARRSetBit(1, 0, 0);

    size_t test2_expected = 1;
    size_t test2 = BARRSetBit(0, 0, 1);

    size_t test3_expected = 23;
    size_t test3 = BARRSetBit(7, 4, 1);

    size_t test4_expected = 0XFFFFFFFFFFFFFFFF;
    size_t test4 = BARRSetBit(0X7FFFFFFFFFFFFFFF, 63, 1);

    if(test1 != test1_expected || test2 != test2_expected || 
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0;
}

int TestSetOn()
{
    size_t test1_expected = 1;
    size_t test1 = BARRSetOn(0, 0);

    size_t test2_expected = 23;
    size_t test2 = BARRSetOn(7, 4);

    size_t test3_expected = 0XFFFFFFFFFFFFFFFF;
    size_t test3 = BARRSetOn(0X7FFFFFFFFFFFFFFF, 63);

    size_t test4_expected = 4194304;
    size_t test4 = BARRSetOn(0, 22);

    if(test1 != test1_expected || test2 != test2_expected || 
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0;
}

int TestSetOff()
{
    size_t test1_expected = 0;
    size_t test1 = BARRSetOff(1, 0);

    size_t test2_expected = 7;
    size_t test2 = BARRSetOff(23, 4);

    size_t test3_expected = 0X7FFFFFFFFFFFFFFF;
    size_t test3 = BARRSetOff(0XFFFFFFFFFFFFFFFF, 63);

    size_t test4_expected = 0;
    size_t test4 = BARRSetOff(4194304, 22);

    if(test1 != test1_expected || test2 != test2_expected || 
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0;
}

int TestIsOn()
{
    size_t test1 = BARRIsOn(1, 0); /* expected: !0 */
    size_t test2 = BARRIsOn(23, 1); /* expected: !0 */
    size_t test3 = BARRIsOn(0X0FFFFFFFFFFFFFFF, 63); /* expected: 0 */
    size_t test4 = BARRIsOn(4194304, 22); /* expected: !0 */

    if(0 == test1 || 0 == test2 || 0 != test3 || 0 == test4)
    {
        return 1;
    }

    return 0;
}

int TestIsOff()
{
    size_t test1 = BARRIsOff(1, 0); /* expected: 0 */

    size_t test2 = BARRIsOff(23, 1); /* expected: 0 */

    size_t test3 = BARRIsOff(0X0FFFFFFFFFFFFFFF, 63); /* expected: !0 */

    size_t test4 = BARRIsOff(4194304, 22); /* expected: 0 */
   
    if(0 != test1 || 0 != test2 || 0 == test3 || 0 != test4)
    {
        return 1;
    }

    return 0;
}

int TestFlip()
{
    size_t test1_expected = 0;
    size_t test1 = BARRFlip(1, 0);

    size_t test2_expected = 21;
    size_t test2 = BARRFlip(23, 1);

    size_t test3_expected = 0X8FFFFFFFFFFFFFFF;
    size_t test3 = BARRFlip(0X0FFFFFFFFFFFFFFF, 63);


    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected)
    {
        return 1;
    }

    return 0; 
}

int TestCountOn()
{
    size_t test1_expected = 1;
    size_t test1 = BARRCountOn(1);

    size_t test2_expected = 64;
    size_t test2 = BARRCountOn(0xFFFFFFFFFFFFFFFF);
    
    size_t test3_expected = 32;
    size_t test3 = BARRCountOn(0xAAAAAAAAAAAAAAAA);

    size_t test4_expected = 1;
    size_t test4 = BARRCountOn(0x0000000000002000);

    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0; 
}

int TestCountOnLUT()
{
    size_t test1_expected = 1;
    size_t test1 = BARRCountOnLUT(1);

    size_t test2_expected = 64;
    size_t test2 = BARRCountOnLUT(0xFFFFFFFFFFFFFFFF);
    
    size_t test3_expected = 32;
    size_t test3 = BARRCountOnLUT(0xAAAAAAAAAAAAAAAA);

    size_t test4_expected = 1;
    size_t test4 = BARRCountOnLUT(0x0000000000002000);

    size_t test5_expected = 1;
    size_t test5 = g_count_lut_counter;

    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected ||
       test5 != test5_expected)
    {
        return 1;
    }

    return 0; 
}

int TestCountOff()
{
    size_t test1_expected = 63;
    size_t test1 = BARRCountOff(1);

    size_t test2_expected = 0;
    size_t test2 = BARRCountOff(0xFFFFFFFFFFFFFFFF);
    
    size_t test3_expected = 32;
    size_t test3 = BARRCountOff(0xAAAAAAAAAAAAAAAA);

    size_t test4_expected = 63;
    size_t test4 = BARRCountOff(0x0000000000002000);

    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0; 
}

int TestMirror()
{
    size_t test1_expected = 0x8000000000000000;
    size_t test1 = BARRMirror(1);

    size_t test2_expected = 0x0000000011111111;
    size_t test2 = BARRMirror(0X8888888800000000);
    
    size_t test3_expected = 0x1579B57912341234;
    size_t test3 = BARRMirror(0X2C482C489EAD9EA8);

    size_t test4_expected = 0x117DB77912741234;
    size_t test4 = BARRMirror(0x2C482E489EEDBE88);

    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0; 
}

int TestMirrorLUT()
{
    size_t test1_expected = 0x8000000000000000;
    size_t test1 = BARRMirrorLUT(1);

    size_t test2_expected = 0x0000000011111111;
    size_t test2 = BARRMirrorLUT(0X8888888800000000);
    
    size_t test3_expected = 0x1579B57912341234;
    size_t test3 = BARRMirrorLUT(0X2C482C489EAD9EA8);

    size_t test4_expected = 0x117DB77912741234;
    size_t test4 = BARRMirrorLUT(0x2C482E489EEDBE88);

    size_t test5_expected = 0x117DB77912741234;
    size_t test5 = BARRMirrorLUT(0x2C482E489EEDBE88);


    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected ||
       test5 != test5_expected)
    {
        return 1;
    }

    return 0; 
}

int TestRotateLeft()
{
    size_t test1_expected = 0x0000000000000002;
    size_t test1 = BARRRotateLeft(1, 1);

    size_t test2_expected = 0XBCDABcDFFFFFFFFA;
    size_t test2 = BARRRotateLeft(0XABCDABCDFFFFFFFF, 4);
    
    size_t test3_expected = 0X1122334455667700;
    size_t test3 = BARRRotateLeft(0X0011223344556677, 8);

    size_t test4_expected = 0x0000000000000002;
    size_t test4 = BARRRotateLeft(1, 65);

    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0; 
}

int TestRotateRight()
{
    size_t test1_expected = 1;
    size_t test1 = BARRRotateRight(0x0000000000000002, 1);

    size_t test2_expected = 0XABCDABCDFFFFFFFF;
    size_t test2 = BARRRotateRight(0XBCDABcDFFFFFFFFA, 4);
    
    size_t test3_expected = 0X0011223344556677;
    size_t test3 = BARRRotateRight(0X1122334455667700, 8);

    size_t test4_expected = 1;
    size_t test4 = BARRRotateRight(0x0000000000000002, 65);

    if(test1 != test1_expected || test2 != test2_expected ||
       test3 != test3_expected || test4 != test4_expected)
    {
        return 1;
    }

    return 0; 
}

int main()
{
    RUN_TEST(TestSet);
    RUN_TEST(TestSetOn);
    RUN_TEST(TestSetOff);
    RUN_TEST(TestIsOn);
    RUN_TEST(TestIsOff);
    RUN_TEST(TestFlip);
    RUN_TEST(TestCountOn);
    RUN_TEST(TestCountOff);
    RUN_TEST(TestMirror);
    RUN_TEST(TestRotateLeft);
    RUN_TEST(TestRotateRight);
    RUN_TEST(TestCountOnLUT);
    RUN_TEST(TestMirrorLUT);

    return 0;
}
