#include <stdio.h>
#include <stdlib.h>
#include "ws3.h"

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
    } \
    

void test_clear(char **envp)
{

    char **copy = calloc(sizeof(char **), 500);

    CopyEnv(envp, copy);

    PrintEnv(copy);

    CleanEnv(copy); 
}

void test_soldiers()
{   
    int num_of_soldiers = 100;
    int *soldiers= calloc(sizeof(int), num_of_soldiers);

    int result = -23;

    SoldierArrayInit(soldiers, num_of_soldiers);

    result = SoldierFind_N(soldiers, num_of_soldiers);

    printf("%d\n", result);
}

void test_copy(char **envp)
{
    int num_of_envps = NumOfEnvp(envp);

    char **copy = calloc(sizeof(char **), num_of_envps + 1);

    CopyEnv(envp, copy);

    PrintEnv(copy);

    CleanEnv(copy); 
}

int main(int argc, char *argv[], char **envp)
{

    /* get rid of unused parameter warning during compilation */ 
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(envp);
    

    PrintTypeSizes();

 
   

   return 0;
}

