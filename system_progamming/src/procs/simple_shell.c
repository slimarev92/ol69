/*******************************************************
*                    .--.                              *
*                    |__| .-------.                    *
*                    |=.| |.-----.|                    *
*                    |--| || >$  ||                    *
*                    |  | |'-----'|                    *
*                    |__|~')_____('                    *
*                                                      *
*            SYSTEM PROGRAMMING: SIMPLE SHELL          *
*            CODE BY:            SASHA LIMAREV         *
*            REVIEWED BY:        MISHEL LIBERMAN       * 
*                                                      *
*******************************************************/
#include <unistd.h> /* fork */
#include <sys/wait.h> /* wait */
#include <stdio.h> /* printf */
#include <stdlib.h> /* system */
#include <string.h> /* strcmp */

#define RESET_COLOR "\x1b[0m"
#define COLOR  "\x1b[32m"

#define INPUT_LEN (100)
#define ARGS_LEN (20)
#define PROG_NAME (0)

static void FreeAll(char **args, char *input);
static void TokenizeInput(char **args, char *input);
static void RunShell();

static void FreeAll(char **args, char *input)
{
    free(args);
    free(input);
}

static void TokenizeInput(char **args, char *input)
{
    char *tok = NULL;
    tok = strtok(input, " \n");
    *args = tok;
    ++args;

    while(NULL != tok)
    {
        tok = strtok(NULL, " \n");
        *args = tok;
        ++args;
    } 
}

static void RunShell()
{
    int running = 1;
    pid_t child_pid = 0;
    char *input = NULL;
    char **args = NULL;

    input = (char *)malloc(sizeof(char) * INPUT_LEN);
    if(NULL == input)
    {
        return;
    }

    args = (char **)calloc(ARGS_LEN, sizeof(char *));
    if(NULL == args)
    {
        free(input);
        return;
    }
        
    while(running)
    {
        printf("%s%s@OL69:", RESET_COLOR, getenv("USER"));
        printf(" %s$ %s", getenv("PWD"), RESET_COLOR);

        fgets(input, INPUT_LEN, stdin);
        TokenizeInput(args, input);

        if(0 == strlen(args[PROG_NAME]))
        {
            continue;
        }
        else if(0 == strcmp(args[PROG_NAME], "exit"))
        {
            running = 0;
        }
        else
        {
            child_pid = fork();

            if(0 == child_pid)
            {
                execvp(args[PROG_NAME], args);
                exit(-1); /* exit child immediately if something went wrong */
            }
            else
            {
                wait(NULL);
                child_pid = 0;
            }
        }
    }

    FreeAll(args, input);
}

int main()
{
    RunShell();

    return 0;
}