#include <stdio.h> /* printf */
#include <errno.h> /* errno */
#include <stdlib.h> /* calloc */
#include <string.h> /* strerror */

#include "../include/cbuf.h"

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

int TestInitialCapacity()
{
    size_t expected_capacity = 23;
    cbuf_t *test_cbuf = CBUFCreate(expected_capacity);
    size_t actual_capacity = CBUFCapacity(test_cbuf);

    if(actual_capacity != expected_capacity)
    {
        printf("%sExpected: %ld Actual: %ld\n",
               RED, expected_capacity, actual_capacity);

        return 1;
    }
   
    return 0;
}

int TestWriteRead1()
{
    cbuf_t *test_cbuf = CBUFCreate(8);
    char *test_string = "1234567890!@#$%^&*"; 
    char *test_buff = (char *)calloc(sizeof(char), 8);
    int expected_written_bytes = 8;
    int actual_written_bytes = 0;
    int actual_read_bytes = 0;
    int expected_read_bytes = 8;
    int i = 0;
   
    actual_written_bytes = CBUFWrite(test_cbuf, (void *)test_string, 10);

    if(actual_written_bytes != expected_written_bytes)
    {
        printf("%sLine %d - Expected Bytes: %d Actual Bytes: %d\n",
               RED, __LINE__ - 3,expected_written_bytes, actual_written_bytes);
        return 1;
    }

    if(errno != EOVERFLOW)
    {
        printf("%sLine %d - Expected error: %s, Actual error: %s\n",
               RED, __LINE__ - 3, strerror(EOVERFLOW), strerror(errno));
        return 1;
    }
    
    actual_read_bytes = CBUFRead(test_cbuf, (void *)test_buff, 1000);

    if(expected_read_bytes != actual_read_bytes)
    {
         printf("%sLine %d - Expected Bytes: %d Actual Bytes: %d\n",
               RED, __LINE__ - 3,expected_read_bytes, actual_read_bytes);
        return 1;
    }

    if(errno != ENODATA)
    {
        printf("%sLine %d - Expected error: %s, Actual error: %s\n",
               RED, __LINE__ - 3, strerror(ENODATA), strerror(errno));
        return 1;
    }

    for(; i < 8; ++i)
    {
        if(test_string[i] != test_buff[i])
        {
            printf("%sLine %d - Expected Char: %c Actual Char: %c\n",
               RED, __LINE__ - 3, test_string[i], test_buff[i]);
        return 1;
        }
    }

    return 0;
}

int TestWriteRead2()
{
    cbuf_t *test_cbuf = CBUFCreate(8);
    char *test_string = "1234567890!@#$%^&*"; 
    char *test_buff = (char *)calloc(sizeof(char), 8);
    int i = 0;
   
    CBUFWrite(test_cbuf, (void *)test_string, 2);
    CBUFRead(test_cbuf, (void *)test_buff, 1000);

    CBUFWrite(test_cbuf, (void *)test_string, 5);
    CBUFRead(test_cbuf, (void *)test_buff, 5);

    for(; i < 5; ++i)
    {
        if(test_string[i] != test_buff[i])
        {
            printf("%sLine %d - Expected Char: %c Actual Char: %c\n",
               RED, __LINE__ - 3, test_string[i], test_buff[i]);
        return 1;
        }
    }

    return 0;
}


int main()
{
    RUN_TEST(TestInitialCapacity);
    RUN_TEST(TestWriteRead1);
    RUN_TEST(TestWriteRead2);
   
    
    return 0;
}
