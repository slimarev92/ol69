#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */

#include "kt.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"
#define PURPLE "\x1b[35m"
#define ORANGE "\x1b[33m"
#define CYAN "\x1b[34m"

#define FIRST (0)
#define BOARD_WIDTH (8)
#define RUN_TIME (180)
#define BOARD_SIZE (64)
#define FULL_BOARD ((size_t)0 - 1)

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

static int IsPathValid(int *path)
{
    unsigned char i = 0;
    unsigned char flag = 0;
    unsigned char *list = (unsigned char *)calloc(BOARD_SIZE,
                                                         sizeof(unsigned char));

    for(; BOARD_SIZE > i; ++i)
    {
        if(0 != list[path[i]])
        {

            flag = 1;
        }
        else
        {
            list[path[i]] = 1;
        }
    }

    for(i = 0; BOARD_SIZE > i; ++i)
    {
        if(0 == list[i])
        {
            flag =  1;
        }
    }

    free(list);

    return flag;
}

static int RunKT(int first)
{
    size_t i = 0;
    size_t flag = 0;
    int *path = malloc(sizeof(int) * BOARD_SIZE);
    if(NULL == path)
    {
        return M_FAIL;
    }

    printf("\tStarting from cell %d\n", first);
    KTAdvanced(path, first);

    flag = IsPathValid(path);

    for(i = 0; i < BOARD_SIZE; ++i)
    {
        printf("%d->", path[i]);
    }
    printf("\n");
    printf("------------------------------------------\n");

    if(0 != flag)
    {
        free(path);
        return 1;
    }

    free(path);
    return flag;
}

static int TestKTAdvanced()
{
    size_t i = 0;
    size_t flag = 0;

    for(i = 0; BOARD_SIZE > i; ++i)
    {
        flag = RunKT(i);

        if(0 != flag)
        {
            return 1;
        }
    }

    return 0;
}

/*static int TestKT()
{
    size_t i = 0;
    int *path = malloc(sizeof(int) * BOARD_SIZE);
    if(NULL == path)
    {
        return M_FAIL;
    }

    printf("\tStarting from cell FIRST\n");
    KT(path, FIRST);

    for(i = 0; i < BOARD_SIZE; ++i)
    {
        printf("%d->", path[i]);
    }
    printf("\n");


    free(path);
    return 0;
}*/

int main()
{
    RUN_TEST(TestKTAdvanced);

    return 0;
}
