/****************************
* C-WS3                     *
* CODE BY: SASHA LIMAREV    *
* REVIEWED BY: ERAN SEGAL   *                      
****************************/ 

#include <stdio.h> /* printf */
#include <stdlib.h> /* calloc */
#include <ctype.h> /* tolower */
#include <string.h> /* strlen */
#include "ws3.h"

void PrintTypeSizes()
{
    printf("* Sizes of all primitive types in bytes: \n");
    printf("* Char : %ld \n", sizeof(char));
    printf("* Signed Char : %ld \n",  sizeof(signed char));
    printf("* Unsigned Char : %ld \n",  sizeof(unsigned char));
    printf("* Short : %ld \n",  sizeof(short));
    printf("* Unsigned Short : %ld \n",  sizeof(unsigned short));
    printf("* Int : %ld \n",  sizeof(unsigned int));
    printf("* Long : %ld \n",  sizeof(long));
    printf("* Unsigned Long : %ld \n",  sizeof(unsigned long));
    printf("* Float : %ld \n",  sizeof(float));   
    printf("* Double : %ld \n",  sizeof(double));  
    printf("* Long Double : %ld \n",  sizeof(long double));
    printf("* Pointer: %ld \n", sizeof(int *));
    
}


void PrintEnv(char **envp)
{
    while(NULL != *envp)
    {
        printf("%s\n", *envp);

        ++envp;
    }
}

int NumOfEnvp(char **envp)
{
    int count = 0;

    while(NULL != *(envp + count))
    {
        ++count;
    }
  
    return count;
}

void CleanEnv(char **envp)
{
    char **envp_cpy = envp;

    while(NULL != *envp_cpy)
    {
        free(*envp_cpy);
        *envp_cpy = NULL;
        ++envp_cpy;
    }

    free(envp);
}

void CopyEnv(char **envp, char **envp_dest)
{
    char **dest_runner = envp_dest;

    char **envp_runner = envp;

    while(NULL != *envp_runner) /* run on all env vars */
    {
       
        char *curr_var = *envp_runner;
        char *curr_var_dest = (char *) malloc(sizeof(char) * (strlen(curr_var) + 1));
       
        char *curr_env_start = curr_var_dest;

        if(NULL == curr_var_dest)
        {
            CleanEnv(envp);
            break;
        }
     
        while('\0' != *curr_var)
        {
            *curr_var_dest = tolower(*curr_var);
            ++curr_var;
            ++curr_var_dest;
        }

        *curr_var_dest = '\0';

        *dest_runner = curr_env_start;
       
        ++dest_runner;
        ++envp_runner;
    }

    *dest_runner = NULL;
}

/* find first living soldier to the right of curr_soldier,
   return their position in the array  - O(N * log N)  */
int LivingRight(int soldiers[], int num_of_soldiers, int curr_soldier)
{
    int result = (curr_soldier + 1) >= num_of_soldiers ? 0 : (curr_soldier + 1);

    while(0 != soldiers[result])
    {
        result = (result + 1) >= num_of_soldiers ? 0 : (result + 1);
    }

    return result;
}

/* stab soldier at the num_of_soldier position in the array  - O(N) */
void StabSoldier(int soldiers[], int num_of_soldier)
{
    soldiers[num_of_soldier] = 1;
}

/* print array of soldiers
  (X means a soldier is alive, 0 means soldier they're dead  - O(N * log N)*/
void DisplaySoldierArray(int soldiers[], int num_of_soldiers)
{
    int i = 0;

    for(i = 0; i < num_of_soldiers; i++)
    {
        char curr_soldier = (soldiers[i] == 0) ? '0' : 'X';
       
        printf("%c ", curr_soldier);
    }

    printf("\n");
}

/* returns index of last living soldier - O(n * log n) */
int SoldierFind(int soldiers[], int num_of_soldiers)
{
    int curr_soldier = 0;
    int soldier_to_stab = LivingRight(soldiers, num_of_soldiers, curr_soldier);

    while(soldier_to_stab != curr_soldier)
    {
        StabSoldier(soldiers, soldier_to_stab);

        curr_soldier = LivingRight(soldiers, num_of_soldiers, curr_soldier);
        soldier_to_stab = LivingRight(soldiers, num_of_soldiers, curr_soldier);    
    }
   
    return curr_soldier;
}

/* initialize soldier array - O(n) */
void SoldierArrayInit(int soldiers[], int num_of_soldiers)
{
    int curr_soldier = 0;

    for(curr_soldier = 0; curr_soldier < (num_of_soldiers - 1); curr_soldier++)
    {
        soldiers[curr_soldier] = (curr_soldier + 1) < num_of_soldiers ? (curr_soldier + 1) : 0;
    }
}

int GetNext(int soldiers[], int soldier)
{
    return soldiers[soldier];
}

/* kill first living soldier to the right of given soldier, returns index of new current soldier */
int KillOne(int soldiers[], int curr_soldier)
{
    int next = GetNext(soldiers, curr_soldier);
    int next_next = GetNext(soldiers, next);

    soldiers[curr_soldier] = next_next;

    return next_next;
}

/* perform num_of_kills kills, return current soldier at the end of the proccess - O(N) */
int KillIter(int soldiers[], int num_of_kills, int curr_soldier)
{
    int i = 0;

    for(i = 0; i < num_of_kills; i++)
    {
        curr_soldier = KillOne(soldiers, curr_soldier);
    }

    return curr_soldier;
}

/* returns index of last living soldier - O(N) */
int SoldierFind_N(int soldiers[], int num_of_soldiers)
{
    int curr_soldier = 0;

    while(1 < num_of_soldiers)
    {
        int num_of_kills = (0 == (num_of_soldiers % 2)) ? (num_of_soldiers / 2) : ((num_of_soldiers / 2) + 1);
   
        curr_soldier = KillIter(soldiers, num_of_kills, curr_soldier);
        
        num_of_soldiers -= num_of_kills;
    }
    
    return curr_soldier;
}




