#include <stdio.h> /* printf */
#include <stdlib.h> /* system */
#include "ws4.h"

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
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \ \

int main(int argc, char *argv[], char **envp)
{

    /* get rid of unused parameter warning during compilation */ 
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(envp);


    PrintLettersLUT();

 

    return 0;
}

