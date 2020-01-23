#include <stdio.h> /* printf */

#include "../include/dvec.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"

#define WORD (8)
#define SIZE_SIMPLE (7)
#define SIZE_COMPLEX (5)

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

int TestCreate()
{
    size_t capacity_expected = 20;
    size_t size_expected = 0;
    dvec_t *test_dvec = DVECCreate(10 , 20); /* size, num */

    if(DVECCapacity(test_dvec) != capacity_expected ||
       DVECSize(test_dvec) != size_expected)
    {
        DVECDestroy(test_dvec);
        return 1;
    }

    DVECDestroy(test_dvec);        
    return 0;
}

int TestPush()
{
    dvec_t *test_dvec = DVECCreate(sizeof(int) , 8); /* size, num */
    size_t expected_end_capacity = 65536;
    size_t end_capacity = 0;
    int i = 0;
    int push_status = -1;

    for(; i < 64000; ++i)
    {
        push_status = DVECPushBack(test_dvec, (void *)&i);
        if(0 != push_status)
        {
            return 1;
        }
    }

    end_capacity = DVECCapacity(test_dvec);

    DVECDestroy(test_dvec);

    if(end_capacity != expected_end_capacity)
    {
        return 1;
    }

    return 0;
}

int TestGet()
{
    dvec_t *test_dvec = DVECCreate(sizeof(long) , 8); /* size, num */
    long i = 0;
    int curr = 0;

    for(; i < 64000; ++i)
    {
        DVECPushBack(test_dvec, (void *)&i);
    }

    for(i = 0; i < 64000; ++i)
    {
        curr = *((int *)DVECGetItemAddress(test_dvec, (size_t) i));
        if(curr != i)
        {
            DVECDestroy(test_dvec);
            return 1;
        }
    }

    DVECDestroy(test_dvec);
    return 0;
}

int TestPop()
{
    dvec_t *test_dvec = DVECCreate(sizeof(long) , 8); /* size, num */
    long i = 0;
    size_t end_capacity = 0;
    size_t end_capacity_expected = 4;

    for(; i < 3; ++i)
    {
        DVECPushBack(test_dvec, (void *)&i);
    }

     DVECPopBack(test_dvec);

    end_capacity = DVECCapacity(test_dvec);

    if(end_capacity != end_capacity_expected)
    {
        return 1;
    }

    DVECDestroy(test_dvec);
    return 0;
}

int TestReserve1()
{
    dvec_t *test_dvec = DVECCreate(sizeof(long) , 8); /* size, num */
    size_t end_capacity = 0;
    size_t end_capacity_expected = 2300;

    DVECReserve(test_dvec, 2300);

    end_capacity = DVECCapacity(test_dvec);

    if(end_capacity != end_capacity_expected)
    {
        return 1;
    }

    DVECDestroy(test_dvec);
    return 0;
}

int TestReserve2()
{
    dvec_t *test_dvec = DVECCreate(sizeof(long) , 8); /* size, num */
    long i = 0;
    size_t end_capacity = 0;
    size_t end_capacity_expected = 11;

    for(; i < 10; ++i)
    {
        DVECPushBack(test_dvec, (void *)&i);
    }

    DVECReserve(test_dvec, 1);

    end_capacity = DVECCapacity(test_dvec);

    if(end_capacity != end_capacity_expected)
    {
        return 1;
    }

    DVECDestroy(test_dvec);
    return 0;
}

int main()
{
    RUN_TEST(TestCreate);
    RUN_TEST(TestPush);
    RUN_TEST(TestGet);
    RUN_TEST(TestPop);
    RUN_TEST(TestReserve1);
    RUN_TEST(TestReserve2);

    return 0;
}
