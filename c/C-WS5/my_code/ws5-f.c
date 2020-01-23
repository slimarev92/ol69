/*****************************
* C-WS5                      *
* CODE BY:     SASHA LIMAREV *   
* REVIEWED BY: ADAM BLAU     *                          
******************************/ 

#include <stdio.h>  /* printf */
#include <stdlib.h> /* exit   */
#include <string.h> /* strcmp */
#include <errno.h>  /* errno  */
#include "ws5.h"

#define RED "\x1b[31m"

#define NUM_OF_LINKS 5
#define IS_RUNNING 1

#define UNUSED(x) (void)(x);

static char *g_input_block = NULL; /* user input goes here */
static char *g_file_path = NULL;   /* file path goes here  */

int main(int argc, char *argv[])
{
    Start(argv, argc);

    return 0;
}

struct chain_link 
{
    char *str;
    int (*cmp_func)(const char *, const char *);
    enum action_status (*oper_func)(char *);
};

struct chain_link GetLink(int i)
{
    static const struct chain_link chain[] = 
    {
        {"-exit\n", StrCmpBool, ExitProg},
        {"-remove\n", StrCmpBool, Remove},
        {"-count\n", StrCmpBool, Count},
        {"append start", PrependCmp, Prepend},
        {"append end", AppendCmp, AppendEnd}
    };

    return chain[i];    
}

int StrCmpBool(const char *s1, const char *s2)
{
    int result = (strcmp(s1, s2) == 0) ? 1 : 0;

    return result;
}

void Start(char *argv[], int argc)
{
    if(2 > argc)
    {
        fprintf(stderr, "%sError - Filename Argument Was Not Supplied.\n", RED);
        exit(1);
    }

    g_file_path = argv[1];

    g_input_block = (char *) malloc(sizeof(char) * 100);
    if(NULL == g_input_block)
    {
        fprintf(stderr, "%sError: Can't Allocate Memory\n", RED);
        exit(1);
    }

    ChainMechanism();
}

void ChainMechanism()
{
    while(IS_RUNNING)
    {
        int i = 0;        

        printf("$ ");
        ReadLine(stdin, g_input_block);
                    
        for(; i < NUM_OF_LINKS; ++i)
        {
            struct chain_link curr_link = GetLink(i);
            int check_input = curr_link.cmp_func(curr_link.str, g_input_block);
                   
            if(0 != check_input)
            {
                enum action_status status = curr_link.oper_func(g_input_block);
                
                ErrorCheck(status);

                break;
            }
        }       
    }
}

void ErrorCheck(enum action_status curr_status)
{
    if(ERROR == curr_status)
    {
        fprintf(stderr, "%sERROR: %s (%s).\n",
                   RED, strerror(errno), g_file_path);
        free(g_input_block);
        exit(1);
    }
}

char *ReadLine(FILE *stream, char *str)
{
    char *str_cpy = str;

    char curr_char = (char) fgetc(stream);

    while('\n' != curr_char)
    {
        *str_cpy = curr_char;
        curr_char = (char) fgetc(stream);
        
        ++str_cpy;
    }

    *str_cpy = '\n';
    ++str_cpy;
    *str_cpy = '\0';

    return str;
}

int CountLines(FILE *stream)
{
    int result = 0;

    char curr_char = 'c';

    curr_char = fgetc(stream);

    while(EOF != curr_char)
    {
        if('\n' == curr_char)
        {
            ++result;      
        }

        curr_char = fgetc(stream); 
    }

    return result;
}

int PrependCmp(const char *input, const char *str)
{
    UNUSED(input) /* gets rid of warning during compilation */

    if('<' != *str)
    {
        return 0;
    }
    
    return 1;
}

int AppendCmp(const char *input, const char *str)
{
    UNUSED(str)  /* gets rid of warning during compilation */
    UNUSED(input)

    return 1;
}

enum action_status ExitProg(char *str)
{
    UNUSED(str) /* gets rid of warning during compilation */

    free(g_input_block);

    exit(0);
}

enum action_status Remove(char *str)
{
    FILE *file_ptr = fopen(g_file_path, "r");

    int remove_status = 0;
    UNUSED(str) /* gets rid of warning during compilation */
    
    if(NULL == file_ptr)
    {
        return ERROR;
    }
    
    fclose(file_ptr);  
  
    remove_status = remove(g_file_path);

    if(0 != remove_status)
    {
        return ERROR;
    }

    return SUCCESS;
}

enum action_status Count(char *str)
{
    FILE *file = fopen(g_file_path, "r+");
    int close_status = 0;

    int line_count = 0;
    
    if(NULL == file)
    {
        return ERROR;
    }

    UNUSED(str)  /* gets rid of warning during compilation */

    line_count = CountLines(file);
    fprintf(file, "%d\n", line_count);
    close_status = fclose(file);

    if(0 != close_status)
    {
        return ERROR;
    }

    return SUCCESS;
}

enum action_status AppendEnd(char *str)
{
    FILE *file = fopen(g_file_path, "a");
    int close_status = 0;
 
    if(NULL == file)
    {
        return ERROR;
    }

    fprintf(file, "%s", str);

    close_status = fclose(file);

    if(0 != close_status)
    {
        return ERROR;
    }

    return SUCCESS;
}

enum action_status CopyFileContents(char *path_src, char *path_dst)
{
    FILE *dst = fopen(path_dst, "w");
    FILE *src = fopen(path_src, "r");

    int close_status_src = 0;
    int close_status_dst = 0;
    int rm_status_temp = 0;

    int curr_char = 0;

    if(NULL == dst || NULL == src)
    {
        fclose(dst);
        fclose(src);
        return ERROR;
    }

    curr_char = fgetc(src);
    
    while(EOF != curr_char)
    {
        fputc(curr_char, dst);
        curr_char = fgetc(src);
    }

    close_status_src = fclose(src);
    close_status_dst = fclose(dst);

    rm_status_temp = remove(".temp.txt");

    if(0 != close_status_src || 0 != close_status_dst || 0 != rm_status_temp)
    {
        return ERROR;
    }

    return SUCCESS;
}

enum action_status Prepend(char *str) /* needs more work */
{
    FILE *file = fopen(g_file_path, "r");
    FILE *temp = fopen(".temp.txt", "a");

    int curr_char = 0;
    int close_status_file = 0;
    int close_status_temp = 0;
 
    if(NULL == file || NULL == temp) /* split this ? */
    {
        return AppendEnd(str+1);
    }

    fprintf(temp, "%s", (str+1));

    curr_char = fgetc(file);

    while(EOF != curr_char) 
    {
        fputc(curr_char, temp);
        curr_char = fgetc(file);
    }

    close_status_file = fclose(file);
    close_status_temp = fclose(temp);
    
    if(0 != close_status_file || 0 != close_status_temp)
    {
        return ERROR;
    }

    return CopyFileContents(".temp.txt", g_file_path);
}
