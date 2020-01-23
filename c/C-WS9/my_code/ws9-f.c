/*****************************
* C-WS9                      *
* CODE BY:     SASHA LIMAREV *   
* REVIEWED BY: YAEL LOTAN    *                          
******************************/ 

#include <stdio.h>   /* printf  */
#include <stdlib.h> /* calloc  */
#include <ctype.h> /* isdigit */
#include "ws9.h"

#define WORD (size_t) (8)
#define IS_LITTLE_ENDIAN (*(short *) "\x01\x00" == (short) 1)

static char g_LUT[] = 
{
        '0','1','2','3','4','5','6','7','8','9', 'a','b','c','d',
        'e','f','g','h','i','j','k','l','m','n', 'o','p','q','r',
        's', 't','u','v','w','x','y','z'
};

void *MemSet(void *s, int c, size_t n)
{
    size_t i = 0;

    size_t part1_iters = CalculatePart1Iters(n, (size_t) s);
    size_t part2_iters = (n - part1_iters) / WORD;
    size_t part3_iters =  n - (part2_iters * WORD) - part1_iters;

    char *part1_ptr = (char *)s;
    long *part2_ptr = (long *)(part1_ptr + part1_iters);
    char *part3_ptr = (char *)part2_ptr + (part2_iters * WORD);

    long block = 0;
    char *ch_ptr = (char *)&block;

    for(; i < WORD; ++i) 
    {
        *(ch_ptr + i) = (char)c;
    }

    for(i = 0; i < part1_iters; ++i)
    {
        *(part1_ptr + i) = (char)c;
    }

    for(i = 0; i < part2_iters; ++i)
    {   
        *(part2_ptr + i) = block;
    }

    for(i = 0; i < part3_iters; ++i)
    {      
        *(part3_ptr + i) = (char)c;
    }
 
    return s;
}

void *MemCpy(void *dest, const void *src, size_t n)
{
    size_t i = 0;

    const char *src_cpy = src;

    size_t part1_iters = CalculatePart1Iters(n, (size_t)dest);
    size_t part2_iters = (n - part1_iters) / WORD;
    size_t part3_iters =  n - (part2_iters * WORD) - part1_iters;

    char *part1_ptr = (char *)dest;
    long *part2_ptr = (long *)(part1_ptr + part1_iters);
    char *part3_ptr = (char *)part2_ptr + (part2_iters * WORD);

    printf("p1: %d p2: %d p3 :%d\n", (int)part1_iters,
                  (int) part2_iters, (int)part3_iters);

    for(i = 0; i < part1_iters; ++i)
    {
        *(part1_ptr + i) = *src_cpy;
        ++src_cpy;
    }

    for(i = 0; i < part2_iters; ++i)
    {   
        *(part2_ptr + i) = *((long *)src_cpy);
        src_cpy += WORD;
    }

    for(i = 0; i < part3_iters; ++i)
    {      
        *(part3_ptr + i) = *src_cpy;
        ++src_cpy;
    }
 
    return dest;
}

void *MemMove(void *dest, const void *src, size_t n)
{
    size_t i = 0;

    size_t part1 = CalculatePart1Iters(n, (size_t)dest);
    size_t part2 = (n - part1) / WORD;
    size_t part3 =  n - (part2 * WORD) - part1;

    char *part1_src = (char *)src + part1 - 1;
    long *part2_src = (long *)(part1_src + (part2 * WORD) - 7);
    char *part3_src = (char *)part2_src + WORD + part3 - 1;

    char *part1_ptr = (char *)dest + part1 - 1;
    long *part2_ptr = (long *)(part1_ptr + (part2 * WORD) - 7);
    char *part3_ptr = (char *)part2_ptr + WORD + part3 - 1;
    
    if((size_t)dest + n - 1 <= (size_t)src + n - 1)
    {
        return MemCpy(dest, src, n);
    } 

    for(i = 0; i < part3; ++i)
    {      
        *(part3_ptr - i) = *(part3_src - i);
    }

    for(i = 0; i < part2; ++i)
    {   
        *(part2_ptr - i) = *(part2_src - i);
    }

    for(i = 0; i < part1; ++i)
    {
        *(part1_ptr - i) = *(part1_src - i);
    }

    return dest;
}

int CalculatePart1Iters(int n, size_t dest)
{
    if(n < 8)
    {
        return n;
    }
    else if(0 == (dest % WORD)) 
    {
       return 0;
    }
    else
    {
        return (int)WORD - (dest % WORD);
    }
}

char *Itoa(int num, char *str) 
{
   return ItoaAny(num, str, 10); 
}

char *StrReverse(char *str, int len)
{
    int i = 0;

    for(; i < len / 2; ++i)
    {
        char temp = *(str + i);
        *(str + i) = *(str + len - i - 1);
        *(str + len - i - 1) = temp;
    }

    return str;
}

int Atoi(const char *str) 
{
    return AtoiAny(str, 10); 
}

int AtoiAny(const char *str, int base) 
{
    int result = 0;
    int num_sign = 1;  

    if(!isdigit(*str))
    {
        num_sign = -1;
        ++str;
    }

    while('\0' != *str)
    {
        int curr_digit = FindLUTIndex(*str);       
        result = (result * base) + curr_digit;     
        ++str;
    }
  
    result *= num_sign;

    return result;
}

int FindLUTIndex(char c)
{
    int result = 0;

    while(c != g_LUT[result])
    {
        ++result;
    }

    return result;
}

char *ItoaAny(int num, char *str, int base) 
{
    char *str_cpy = str;
    int negative_flag = 1;

    if(0 > num)
    {
       num = -num;
       negative_flag = 0;
    }    

    do
    {
        *str_cpy = g_LUT[num % base];
        num /= base;
        ++str_cpy;
    } while(num != 0);

    if(0 == negative_flag)
    {
        *str_cpy = '-';
        ++str_cpy;
    }
    
    *str_cpy = '\0';

    return StrReverse(str, str_cpy - str);
}

void OnlyFirstTwo(char *arr1, char *arr2, char *arr3, int s1, int s2, int s3)
{
    short *histogram = (short *)calloc(256, sizeof(short));
    int i = 0;
    
    for(; i < s1; ++i)
    {
        if(0 == histogram[(int)*(arr1 + i)])
        {
            histogram[(int)*(arr1 + i)] = 1;
        }
    } 

    for(i = 0; i < s2; ++i)
    {
        if(1 >= histogram[(int)*(arr2 + i)])
        {
            histogram[(int)*(arr2 + i)] += 2;
        }
    }  

    for(i = 0; i < s3; ++i)
    {
        if(3 == histogram[(int)*(arr3 + i)])
        {
            histogram[(int)*(arr3 + i)] = -1;
        }
    }      

    for(i = 0; i < 256; ++i)
    {
        if(3  == histogram[i])
        {
            printf("%c\n", (char)i);
        }
    }

    free(histogram);
}

int IsLittleEndian()
{
    unsigned int test = 1;
    char *test_ptr = (char *)&test;

    return (int)*test_ptr;
}
