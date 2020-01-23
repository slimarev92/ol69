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
*     C: WS2                                           *
*     CODE BY: SASHA LIMAREV                           *
*******************************************************/
#include <stdio.h> /* printf */
#include <ctype.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen */

#include "ws2.h"

char *Strtok(char *str, const char *delim)
{
    static char *curr_token = NULL;
    int delim_len = Strlen(delim);
    int i = 0; /* general purpose iterator */
    int extra = 0;
    char *next_delim_left = NULL;
    char *next_delim_right = NULL;
    char *runner = NULL;
    char *result = NULL;

    if(NULL != str)
    {
        int first_delims = Strspn(str, delim);
        curr_token = str + first_delims;
    }

    runner = curr_token;

    while('\0' != *runner 
                && NULL == next_delim_right) 
    {
        
        for(i = 0; i < delim_len; i++) 
        {
            if(*runner == *(delim + i))
            {
                next_delim_left = runner;
                extra = Strspn(next_delim_left, delim) - 1;
                next_delim_right = next_delim_left + extra;   
            }
        }
      
        ++runner;
    }

    runner = ('\0' == *runner) ? runner : runner - 1;

    while(next_delim_left <= next_delim_right && NULL != next_delim_right)
    {
        *next_delim_left = '\0';
        ++next_delim_left;
    } 
    
    result = curr_token;

    if(next_delim_left == next_delim_right && NULL == next_delim_right)
    {
        curr_token = NULL;
        return NULL;
    }
    else 
    {                
        curr_token = runner + extra + 1;
        return result;
    } 
}

/* returns the number corresponding to the given digit character */
int CharToDigit(char c)
{
    return (c - '0');
}

/* returns the character corresponding to the given digit */
char DigitToChar(int d)
{
    return (d + '0');
}

/* shifts string s right by one character */
void ShiftRight(char *s)
{
    char *curr = s + Strlen(s);
  
    while(0 <= (curr - s))
    {
        *(curr + 1) = *curr;
        --curr;
    }
}

void AddBigNums(char *num1, char *num2, char *result)
{
    int len1 = Strlen(num1);
    int len2 = Strlen(num2);

    char *runner_long = NULL;
    char *runner_short = NULL;

    int carry = 0;

    int pos_long = 0;
    int pos_short = 0;

    /* determine which number is longer and set pointers accordingly */
    if(len1 > len2) 
    {
         runner_long = num1;
         runner_short = num2;

         *(result + len1) = '\0';

         pos_long  = len1 - 1;
         pos_short = len2 - 1;
    }
    else 
    {
         runner_long = num2;
         runner_short = num1;

         *(result + len2) = '\0';

          pos_long  = len2 - 1;
          pos_short = len1 - 1;
    }

    while(pos_short >= 0)
    {

        int operand1 = CharToDigit(*(runner_long + pos_long));
        int operand2 = CharToDigit(*(runner_short + pos_short));

        int curr_result =  operand1 + operand2 + carry;

        carry = curr_result / 10;
        
        *(result + pos_long) = DigitToChar(curr_result % 10);

        --pos_short;
        --pos_long;
    }

    while(pos_long >= 0)
     {
        int curr_result =  CharToDigit(*(runner_long + pos_long)) + carry;

        carry = curr_result / 10;

        *(result + pos_long) =  DigitToChar(curr_result % 10);
        
        --pos_long;
     }
    
    if(carry)
    {
        ShiftRight(result);
        *result = '1';
    }
}

int IsWs(char c) /* returns 1 if c is a whitespace charatcer */
{
    return '\t' == c || ' ' == c;
}

/* remove all but leave_ws_chars from the beginning of a string */
int RemoveWsBlock(char *str, int leave_ws_chars)
{
    int chars_before_non_ws = Strspn(str, " \t") - leave_ws_chars;
    char *runner = str + chars_before_non_ws;
    
    while('\0' != *runner)
    {
        *(runner - chars_before_non_ws) = *runner;
        ++runner; 
    }

   *(runner - chars_before_non_ws) = '\0'; 

    return chars_before_non_ws;
}

void CleanWs(char *str)
{
    if(Strspn(str, " \t") == Strlen(str))
    {
        *str = '\0';
        return;
    }

    RemoveWsBlock(str, 0);

    while('\0' != *str)
    {
        if(IsWs(*str) && IsWs(*(str+1)))
        {    
            RemoveWsBlock(str, 1); 
        }
    
        ++str;           
    }

    /* get rid of last whitespace if there's one */
    if(1 == IsWs(*(str - 1)))
    {
         *(str - 1) = '\0'; 
    }
}

char *Strdup(const char *s)
{
    size_t s_len = Strlen(s);
    char *result = (char*)malloc(sizeof(char) * s_len + 1);                 

    while('\0' != *s)
    {
        *result = *s;

        ++s;
        ++result;
    }

    *result = '\0';

    return (result - s_len);

}

size_t Strlen(const char *str) 
{
    const char *runner = str;

    while('\0' != *runner)
    {
        ++runner;
    }

    return runner - str;
}

int Strcasecmp(const char *s1, const char *s2)
{
    while('\0' != *s1)
    { 
        if(*s1 > *s2     && 
         !(*s1 >= 'a'    && /* if *s1 and *s2 are the same */
           *s1 <= 'z'    && /* letter in different cases,  */
           *s1-*s2 == 32))  /* don't enter this block      */
         {
                 return 1; 

        }
        else if(*s1 < *s2     &&  /* if *s1 and *s2 are the same */
              !(*s1 >= 'A'    &&  /* letter in different cases,  */
                *s1 <= 'Z'    &&  /* don't enter this block      */
                *s2-*s1 == 32))
        {
                 return -1; 

        }

        ++s1;
        ++s2;
    }

    if('\0' != *s2)
    {
        return -1; 
    }

    return 0; 
}

int Strcmp(const char *s1, const char *s2)
{
    while('\0' != *s1)
    { 
        if(*s1 > *s2)
        {
            return 1; 
        }
        else if(*s1 < *s2)
        {
            return -1; 
        }

        ++s1;
        ++s2;
    }

    if('\0' != *s2)
    {
        return -1; 
    }

    return 0; 
}

char *Strcpy(char *dest, const char *src)
{
    char *result = dest;

    while('\0' != *src)
    {
        *dest = *src;
        ++src;
        ++dest;
    }

    *dest = '\0';

    return result;
}

char *Strncpy(char *dest, const char *src, size_t n)
{
    char *result = dest;

    while(n > 0)
    {
        if('\0' != *src)
        {
            *dest = *src;
            ++src;

        }

        ++dest;
        --n;
    }

    *dest = '\0';

    return result;
}

char *Strchr(const char *s, int c)
{

    char *runner = (char*) s;

    while('\0' != *runner)
    {

        if(*runner == (char) c)
        {
            return runner;
        }
    
        runner++;    
  
    }

    return NULL;
}

char *Strcat(char *dest, const char *src)
{
    char *dest_start = dest; /* this will be returned */
    
    while('\0' != *dest) /* reach terminating char of dest */
    {
        ++dest;
    } 

    *dest = ' ';
    ++dest;

    while('\0' != *src)
    {
        *dest = *src;
        ++dest;
        ++src;
    }

    *dest = '\0';
    
    return dest_start;
}

char *Strncat(char *dest, const char *src, size_t n)
{
    char *dest_start = dest; /* this will be returned */
    
    while('\0' != *dest) /* reach terminating char of dest */
    {
        ++dest;
    } 

    *dest = ' ';
    ++dest;

    while(0 != n)
    {
        if('\0' == *src)
        {
            break;
        }

        *dest = *src;

        ++dest;
        ++src;
        --n;
    }

    *dest = '\0';
    
    return dest_start;
}

char *Strstr(char *haystack, const char *needle) 
{

    const char *needle_start = needle;
    char *result = NULL;

    while('\0' != *haystack)
    {   
        needle = needle_start;
                
        while(*needle != *haystack)
        {
            ++haystack;
        }

        result = haystack;

        while(*needle == *haystack && '\0' != *haystack)
        {
            ++needle;
            ++haystack;
        }
        
        if('\0' == *needle)
        {
            
            return result;
        } 
    }

    return NULL;
}

size_t Strspn(const char *str, const char *accept)
{
    size_t result = 0; /* return value */

    while('\0' != *str)
    {
        /* if NULL is returned there are no more chars from accept in str */
        char *check = Strchr(accept, (int) *str); 

        if(NULL == check) 
        {
            return result;
        }
        else 
        {
            ++str;
            ++result;
        }
    }

    return result;   
}

int IsPalindrome(char *str)
{
    char *left = str;
    char *right = str + Strlen(str) - 1;

    while(left < right)
    {
        if(*left != *right)
        {
            return 0;
        }

        ++left;
        --right;        

    }

    return 1;
}

void SevenBoom(int from, int to)
{

    int i = 0; /* will be used as iterator */
    
    for(i = from; i <= to; ++i)
    {
        int flag1 = i % 7; /* if i devides by 7 this will be set to 0 */
        int flag2 = 100; /* if i contains 7 this will later be set to 0 */
        
        int i_cpy = 0; /* will be used as copy of i */
        
        for(i_cpy = i; i_cpy > 0; i_cpy /= 10)
        {
            if((i_cpy % 10) == 7)
            {
                flag2 = 0;
                break;
            }
        }
        
        if(flag1 == 0 || flag2 == 0) /* if either flag is 0 print boom */
        {
            printf("BOOM\n");
        }
        else
        {
            printf("%d\n", i);
        }
    }
}

void SwapPtr(int **ptr1, int **ptr2)
{
    int *temp = *ptr2;
    *ptr2 = *ptr1;
    *ptr1 = temp;
}


