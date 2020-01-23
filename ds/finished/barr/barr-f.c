/*****************************
* DS-BARR                    *
* CODE BY:     SASHA LIMAREV *   
* REVIEWED BY: SOMEONE       *                          
******************************/ 

#include <stdio.h>    /* printf  */
#include <assert.h>  /* assert  */
#include <stdlib.h> /* malloc  */
#include "barr.h"

#define BYTE (8)

int g_count_lut_counter = 0;
int g_mirror_lut_counter = 0;

static void DisplayBits(size_t var); 
static int IsIndexWithinBounds(int index);
static int IsLegalValue(int value);
static int *CreateLUTCount();
static size_t *CreateLUTMirror();

size_t BARRCountOnLUT(size_t arr); /* dont add to header yet */

size_t BARRSetBit(size_t arr, int index, int value) 
{  
    assert(IsLegalValue(value));
    assert(IsIndexWithinBounds(index));

    return (0 == value) ? (arr & ~(1ul << index)) : (arr | (1ul << index));
}

size_t BARRSetOn(size_t arr, int index)
{
    return BARRSetBit(arr, index, 1);
}

size_t BARRSetOff(size_t arr, int index)
{
    return BARRSetBit(arr, index, 0);
}

int BARRIsOn(size_t arr, int index)
{ 
    assert(IsIndexWithinBounds(index));

    return arr & (1ul << index);
}

int BARRIsOff(size_t arr, int index)
{
    return !(BARRIsOn(arr, index));
}

size_t BARRFlip(size_t arr, int index)
{
    size_t mask = 1ul << index;

    assert(IsIndexWithinBounds(index));

    return (0 == (mask & arr)) ? (arr | mask) : (arr & ~mask);
}

size_t BARRCountOn(size_t arr) 
{
    size_t result = 0;

    size_t mask1 = 0x5555555555555555;
    size_t mask2 = 0x3333333333333333;
    size_t mask3 = 0x0F0F0F0F0F0F0F0F;
    size_t mask4 = 0x00FF00FF00FF00FF;
    size_t mask5 = 0x0000FFFF0000FFFF;
    size_t mask6 = 0x00000000FFFFFFFF;

    result = (mask1 & arr) + (mask1 & (arr >> 1));
    result = (mask2 & result) + (mask2 & (result >> 2));
    result = (mask3 & result) + (mask3 & (result >> 4));
    result = (mask4 & result) + (mask4 & (result >> 8));
    result = (mask5 & result) + (mask5 & (result >> 16));
    result = (mask6 & result) + (mask6 & (result >> 32));

    return result;
}

size_t BARRCountOnLUT(size_t arr)
{
    size_t count = 0;
    int i = 0;
    static int *ByteCountLUT = NULL;
    size_t mask = 0xFF;

    if(NULL == ByteCountLUT)
    {
        ByteCountLUT = CreateLUTCount();
    }

    for(; i < sizeof(size_t); ++i)
    {
        count += ByteCountLUT[(arr & (mask << (i * BYTE))) >> (i * BYTE)];
    }    

    return count;
}

static int *CreateLUTCount()
{
    int i = 0;
    static int *LUT = NULL;

    LUT = (int *)malloc(sizeof(int) * 256);
    if(NULL == LUT)
    {
        exit(1);
    }

    for(; 256 > i; ++i)
    {
        LUT[i] = BARRCountOn((size_t)i);
    }
    
    ++g_count_lut_counter;
    
    return LUT;
}

size_t BARRCountOff(size_t arr)
{
    return (sizeof(size_t) * BYTE) - BARRCountOn(arr);
}

size_t BARRMirror(size_t arr)
{
    size_t result = 0;
    
    size_t mask0 = 0xFFFFFFFF00000000;
    size_t mask1 = 0xFFFF0000FFFF0000;
    size_t mask2 = 0xFF00FF00FF00FF00; 
    size_t mask3 = 0xF0F0F0F0F0F0F0F0;
    size_t mask4 = 0xCCCCCCCCCCCCCCCC; 
    size_t mask5 = 0xAAAAAAAAAAAAAAAA;

    result = ((mask0 & arr) >> 32) | ((~mask0 & arr) << 32);
    result = ((mask1 & result) >> 16) | ((~mask1 & result) << 16);
    result = ((mask2 & result) >> 8) | ((~mask2 & result) << 8);
    result = ((mask3 & result) >> 4) | ((~mask3 & result) << 4);
    result = ((mask4 & result) >> 2) | ((~mask4 & result) << 2);
    result = ((mask5 & result) >> 1) | ((~mask5 & result) << 1);
  
    return result;
}

size_t BARRMirrorLUT(size_t arr)
{
    int start = 0;
    int end = sizeof(size_t) - 1;
    size_t result = 0;
    static size_t *LUT = NULL;    
    static size_t mask = 0xFF;
    size_t temp_start = 0;
    size_t temp_end = 0;


    if(NULL == LUT)
    {
        LUT = CreateLUTMirror();
    }

    while(start < end)
    {
        temp_start = LUT[(arr >> start * BYTE) & mask]  <<  end * BYTE;
        temp_end = LUT[((arr >> end * BYTE) & mask)] <<  start * BYTE;
        result = (result | temp_start) | temp_end;

        ++start;
        --end;
    }

    return result;
}

static size_t *CreateLUTMirror()
{
    int i = 0;
    static size_t *LUT = NULL;

    LUT = (size_t *)malloc(sizeof(size_t) * 256);
    if(NULL == LUT)
    {
        exit(1);
    }

    for(; 256 > i; ++i)
    {
        LUT[i] = BARRMirror(i) >> (sizeof(size_t) - 1) * BYTE;
    }
    
    ++g_mirror_lut_counter;
    
    return LUT;
}

size_t BARRRotateLeft(size_t arr, size_t amount)
{
    amount = amount % (sizeof(size_t) * BYTE);

    return arr << amount | arr >> (sizeof(size_t) * BYTE - amount);
}

size_t BARRRotateRight(size_t arr, size_t amount)
{
    amount = amount % (sizeof(size_t) * BYTE);

    return arr >> amount | arr << (sizeof(size_t) * BYTE - amount);
}

static int IsIndexWithinBounds(int index)
{
     return ((sizeof(size_t) * BYTE) > (size_t) index) && (0 <= index);
}

static int IsLegalValue(int value)
{
    return (0 == value) | (1 == value);
}

static void DisplayBits(size_t var) 
{
    size_t mask = 1;
    size_t i = 0;

    size_t var_size = sizeof(size_t);
   
    mask = mask << ((var_size * 8) - 1);

    for(; i < var_size * 8; ++i)
    {
        size_t curr_bit = mask & var;

        if(0 == (i % 8) && 0 < i)
        {
            printf("-");
        }
        
        if(0 != curr_bit)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }

        mask = mask >> 1;
    }

    printf("\n");
}    
