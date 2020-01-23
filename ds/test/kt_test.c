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

#define FIRST (20)
#define BOARD_WIDTH (8)
#define RUN_TIME (10000)
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

static int TestKT()
{
    size_t i = 0;
    int *path = malloc(sizeof(int) * BOARD_SIZE);
    if(NULL == path)
    {
        return M_FAIL;
    }

    printf("Starting from cell %ld\n", FIRST);
    KT(path, FIRST);

    for(i = 0; i < BOARD_SIZE; ++i)
    {
        printf("%d->", path[i]);
    }
    printf("\n");


    free(path);
    return 0;
}

int main()
{
    RUN_TEST(TestKT);

    return 0;
}
