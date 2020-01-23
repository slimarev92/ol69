/********************************
* DS:          IP ADDRESS       *
* CODE BY:     SASHA LIMAREV    *   
* REVIEWED BY: ROI KRAMER       *                            
********************************/
#include <assert.h> /* assert */
#include <string.h> /* strtok */
#include <stdlib.h> /* atoi */
#include <stdio.h> /* sprintf */

#include "ip.h"

#define BYTE (8)

void IPArrToStr(ip_t ip, const char *str)
{
    assert(str);

    sprintf((char *)str, "%d.%d.%.d.%d", ip.ip_addr[0], ip.ip_addr[1], 
                                                  ip.ip_addr[2], ip.ip_addr[3]);
}

int IPStrToArr(const char *str, ip_t *res)
{
    size_t i = 0;
    char *str_cp = NULL;
    const char *delim = ".";
    char *curr = NULL;

    assert(str);
    assert(res);

    str_cp = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    if(NULL == str_cp)
    {
        return 1;
    }

    strcpy(str_cp, str);

    for(; IPV > i; ++i)
    {
        curr = 0 == i ? str_cp : NULL;

        res->ip_addr[i] = (unsigned char)atoi(strtok(curr, delim));
    }

    free(str_cp);
    return 0;
}

int IPIsSameIP(const ip_t ip1, const ip_t ip2)
{
    size_t i = 0;

    for(; i < IPV; ++i)
    {
        if(ip1.ip_addr[i]  != ip2.ip_addr[i])
        {
            return 0;
        }
    }

    return 1;
}

uint32_t IPArrToNum(const ip_t ip)
{
    uint32_t res = 0;

    res |= (uint32_t)(ip.ip_addr[0] << 24);
    res |= (uint32_t)(ip.ip_addr[1] << 16);
    res |= (uint32_t)(ip.ip_addr[2] << 8);
    res |= (uint32_t)(ip.ip_addr[3]);

    return res;
}

/* only works with little endian systems */
ip_t IPNumToArr(uint32_t num)
{   
    ip_t res = {{0, 0, 0, 0}};

    res.ip_addr[0] = (char)(num >> 24);
    res.ip_addr[1] = (char)((num << 8) >> 24);
    res.ip_addr[2] = (char)((num << 16) >> 24);
    res.ip_addr[3] = (char)((num << 24) >> 24);

    return res;
}

void DisplayIP(ip_t ip)
{
    size_t i = 0;

    for(; IPV > i; ++i)
    {
        printf("%u", ip.ip_addr[i]);
        if(IPV - 1 != i)
        {
            printf(".");
        }
    }

    printf("\n");
}

void DisplayBits(uint32_t var) 
{
    size_t mask = 1;
    size_t i = 0;

    size_t var_size = sizeof(uint32_t);
   
    mask = mask << ((var_size * BYTE) - 1);

    for(; i < var_size * BYTE; ++i)
    {
        size_t curr_bit = mask & var;

        if(0 == (i % BYTE) && 0 < i)
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
