/********************************
* DS:          DHCP             *
* CODE BY:     SASHA LIMAREV    *   
* REVIEWED BY: ROI KRAMER       *                            
********************************/
#include <assert.h> /* assert */
#include <math.h> /* pow */
#include <stdlib.h> /* malloc */

#include "ip.h" /* IPArrToNum */
#include "trie.h" /* TrieInsertClose */

#include "dhcp.h"

#define BYTE (8)
#define MAX_UINT32 (0xFFFFFFFF)

struct dhcp
{
    trie_t *ip_pool;
    ip_t network_addr;
    uint32_t subnet_mask;
};

static uint32_t GetNumOfAdrs(const dhcp_t *dhcp);
static int IsFull(dhcp_t *dhcp);

enum dhcp_status DHCPGetAddr(dhcp_t *dhcp, ip_t ip_req, ip_t *ip_res)
{
    uint32_t ip_as_num = 0;
    uint32_t res = 0;
    uint32_t m_cpy = 0;
    enum trie_status status = FULL;

    assert(dhcp);
    assert(ip_res);

    m_cpy = dhcp->subnet_mask;
    ip_as_num = IPArrToNum(dhcp->network_addr) | (IPArrToNum(ip_req) & ~m_cpy);

    if(!IsFull(dhcp))
    {
        status = TrieInsertClose(dhcp->ip_pool, (void *)&ip_as_num, &res);
        *ip_res = IPNumToArr(res);
    }

    switch(status)
    {
        case FULL:
            return NO_FREE_ADDR;

        case ADDED_EXIST:
            return ADDR_TAKEN;

        default:
            return OK;
    }
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(dhcp);

    if(MAX_UINT32 == dhcp->subnet_mask)
    {
        return 0;
    }
    else
    {
        return (GetNumOfAdrs(dhcp) + 1) - TrieCount(dhcp->ip_pool);
    }
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(dhcp);

    TrieDestroy(dhcp->ip_pool);
    dhcp->ip_pool = NULL;

    free(dhcp);
    dhcp = NULL;
}

static uint32_t GetNumOfAdrs(const dhcp_t *dhcp)
{
    return (uint32_t)MAX_UINT32 - dhcp->subnet_mask;
}

int DHCPFreeAddr(dhcp_t *dhcp, ip_t ip_addr)
{
    uint32_t to_remove = 0;

    assert(dhcp);

    to_remove = IPArrToNum(ip_addr);

    return (TrieRemove(dhcp->ip_pool, &to_remove) == REMOVED) ? 0 : 1;
}

dhcp_t *DHCPCreate(ip_t network_addr, uint32_t subnet_mask)
{
    trie_t *trie = NULL;
    ip_t dummy_ip = {{0}};
    uint32_t broadcast = 0;
    dhcp_t *new_dhcp = NULL;

    assert(MAX_UINT32 != subnet_mask);

    new_dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
    if(NULL == new_dhcp)
    {
        return NULL;
    }

    trie = TrieCreate(IPV * BYTE);
    if(NULL == trie)
    {
        free(new_dhcp);
        new_dhcp = NULL;

        return NULL;
    }

    new_dhcp->ip_pool = trie;
    new_dhcp->network_addr = network_addr;
    new_dhcp->subnet_mask = subnet_mask;

    broadcast = IPArrToNum(network_addr) | GetNumOfAdrs(new_dhcp); 

    /* add network address */
    if(OK != DHCPGetAddr(new_dhcp, network_addr, &dummy_ip))
    {
        return NULL;
    }

    /* add broadcast address */
    if(OK != DHCPGetAddr(new_dhcp, IPNumToArr(broadcast), &dummy_ip))
    {
        return NULL;
    }
    
    return new_dhcp;
}

/* returns 1 if dhcp's IP pool is full, 0 otherwise */
static int IsFull(dhcp_t *dhcp)
{
    return 0 == DHCPCountFree(dhcp);
}