/***************************************
* DS:          IP ADDRESS (TESTS)      *
* CODE BY:     SASHA LIMAREV           *   
* REVIEWED BY: SOMEONE                 *                          
****************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcmp */

#include "ip.h"

#define BLANK_IP {{0, 0, 0, 0}}

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

int TestStrToArrToStr()
{
    ip_t test_ip = BLANK_IP;
    ip_t expected_ip = {{192, 168, 1, 17}};
    static char expected_str[] = "192.168.1.17";
    static char actual_str[] = "000.000.000.000"; 

    /* convert string to ip struct */
    IPStrToArr(expected_str, &test_ip);

    /* convert result struct back to string */
    IPArrToStr(test_ip, actual_str);

    /* check if the resulting struct was properly converted back to a string */
    if(0 != strcmp(expected_str, actual_str))
    {
        printf("\t%sLine %d - Expected IP string %s, actual IP string %s\n",
                                    RED, __LINE__ - 4, expected_str,actual_str);
        return 1;
    }

    /* check if resulting struct's values match expected values */
    if(!IPIsSameIP(test_ip, expected_ip))
    {
        
        printf("\t%sLine %d - Expected IP %s, actual IP %s\n",
                                    RED, __LINE__ - 4, expected_str,actual_str);
        return 1;
    }

    DisplayBits(IPArrToNum(test_ip));
    printf("%u\n", IPArrToNum(test_ip));

    return 0;
}

int main()
{
    RUN_TEST(TestStrToArrToStr);

    return 0;
}
