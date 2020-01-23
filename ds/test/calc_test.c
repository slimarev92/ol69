#include <stdio.h> /* printf */

#include "../include/calculator.h" /* bst header file */

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[93m"
#define RESET_COLOR "\x1b[0m"

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

void PrintStatus(int status)
{
    switch(status)
    {
        case 0:
            printf("OK\n");
            break;
        
        case 1:
            printf("E_MATH\n");
            break;

        case 2:
            printf("E_SYNTAX\n");
            break;

        case 3:
            printf("E_MALLOC\n");
            break;
    }
}

int TestCalcRegular()
{
    char *expressions[] = {"(2+2)*2",
                           "2*(2^2^2/2)",
                           "40+2/2-3*2",
                           "40+2/2-(3*(2))",
                           "40+2/2-(3*(2))+20/2^(2+2)-(10/2.5)",
                           "2+(3*5+2^2^1+50/25-13+((23)))"
                          };  
    double ex_res[] =  {8.0, 16.0, 35.0, 35.0, 32.25, 33.0};
    size_t arr_size = 6;
    double actual_results[6];
    size_t i = 0;

    for(; i < arr_size; ++i)
    {
        printf("\tExpression: %s\n", expressions[i]);

        Calculate(expressions[i], &actual_results[i]);
        
        if(actual_results[i] != ex_res[i])
        {
            printf("\t%sLine %d - Expected result %.2f, actual res %.2f\n",
                               RED, __LINE__ - 3, ex_res[i], actual_results[i]);
            return 1;
        }
        else
        {
            printf("\tResult: %.2f\n", actual_results[i]);
            printf("\t--------------------\n");
        }
    }

    return 0;
}

int TestCalcError()
{
    char *expressions[] = { "2+2*3**2",
                            "*40+2/2-3*2",
                            "40+2/2-(3*2))",
                            "50/1/(10*23+1/0)^1000",
                            "(40+2/2-(3*(2))"
                          }; 
    int ex_status[] ={E_SYNTAX, E_SYNTAX, E_SYNTAX, E_MATH, E_SYNTAX};
    double dummy_result = -23.0;
    size_t arr_size = 5;
    int actual_status[5];
    size_t i = 0;

    for(; i < arr_size; ++i)
    {
        printf("\tExpression: %s \n", expressions[i]);

        actual_status[i] = Calculate(expressions[i], &dummy_result);
        
        if(actual_status[i] != ex_status[i])
        {
            printf("\t%sLine %d -Expected status %d, got status\n",
                          RED, __LINE__ - 3, ex_status[i]);
            PrintStatus(actual_status[i]);
            return 1;
        }
        else
        {
            printf("\tStatus: ");
            PrintStatus(ex_status[i]);
            printf("\t--------------------\n");
        }
        
    }

    return 0;
}

int main()
{
    RUN_TEST(TestCalcRegular);
    RUN_TEST(TestCalcError);

    return 0;
}
