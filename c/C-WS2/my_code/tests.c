/*******************************************************
*                                                      *
*      .oooooo.   ooooo            .ooo    .ooooo.     *
*     d8P'  `Y8b  `888'          .88'     888' `Y88.   *
*    888      888  888          d88'      888    888   *
*    888      888  888         d888P"Ybo.  `Vbood888   *
*    888      888  888         Y88[   ]88       888'   *
*    `88b    d88'  888       o `Y88   88P     .88P'    *
*     `Y8bood8P'  o888ooooood8  `88bod8'    .oP'       *
*                                                      *
*     C: WS2 (TEST)                                    *
*     CODE BY: SASHA LIMAREV                           *
*******************************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "ws2.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"

#define RUN_TEST(test_name) \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfuly]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfuly]\n", RED); \
    } \
    

int test_strtok()
{
    char *str = (char *)malloc(sizeof(char) * 100);
    char *curr_token1 = NULL;
    char *delim = "*;";

    Strcpy(str, "this****is***a*;*test****");

    curr_token1 = Strtok(str, delim);

    while(NULL != curr_token1)
    {
        printf("%s \n", curr_token1);

        curr_token1 = Strtok(NULL, delim);
    }

    free(str);
    
    return 0;
}

int test_clean_ws()
{

    char *test1 = (char *)malloc(sizeof(char) * 100);
    char *test1_expected = "my name is sasha *";
    int test1_flag = 0;

    char *test2 = (char *)malloc(sizeof(char) * 100);
    char *test2_expected = "";
    int test2_flag = 0;

    Strcpy(test1, "   my  name   is sasha *   ");
    Strcpy(test2, "     \t\t\t\t\t");
    
    CleanWs(test2);
    CleanWs(test1);

    test1_flag = Strcmp(test1, test1_expected);                                
    test2_flag = Strcmp(test2, test2_expected);

    free(test1);
    free(test2);

    if(0 == test1_flag && 0 == test2_flag)
    {
        return 0;
    }
    else 
    {
        return -1;
    }
}

int test_big_num()
{
    char *test1_num1 = "45676545";
    char *test1_num2 = "876556765";
    const char *test1_expected = "922233310";

    char *test1_result = (char *)malloc(sizeof(char) * 10);

    int flag1 = 1000;
    
    AddBigNums(test1_num1, test1_num2, test1_result);

    flag1 = Strcmp(test1_result, test1_expected);

    free(test1_result);

    if(0 != flag1)
    {
        return -1;
    }
    else 
    {
        return 0;
    }
}

int main()
{
    RUN_TEST(test_strtok);
    RUN_TEST(test_big_num);
    RUN_TEST(test_clean_ws);

    return 0;
}

