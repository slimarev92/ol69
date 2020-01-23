/***************************************
* DS:          IP ADDRESS (TESTS)      *
* CODE BY:     SASHA LIMAREV           *   
* REVIEWED BY: ROI KRAMER       *                          
****************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h> /* time */
#include <string.h> /* strcmp */

#include "ip.h"
#include "dhcp.h"

#define BLANK_IP {{0, 0, 0, 0}}

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define YELLOW "\x1b[93m"
#define BLUE "\x1b[36m"
#define PURPLE "\x1b[35m"
#define ORANGE "\x1b[33m"
#define CYAN "\x1b[34m"

#define RUN_TEST(test_name) \
    printf("\n%s[Calling: " #test_name "]\n%s", YELLOW, RESET_COLOR); \
    \
    if(0 == test_name()) \
    { \
        printf("%s[" #test_name " was completed successfully]\n", GREEN); \
    } \
    else \
    { \
        printf("%s[" #test_name " was completed unsuccessfully]\n", RED); \
    } \
    printf("%s\n", RESET_COLOR);

static int TestCreateDestroy()
{
    ip_t test_net = {{0}};
    uint32_t test_mask = 0xFFFFFF00; /* 255.255.255.0 */
    dhcp_t *test_dhcp = NULL;
    size_t actual_count = 0;
    size_t expected_count = 254;

    /* create DHCP */
    IPStrToArr("192.168.1.0", &test_net);
    test_dhcp = DHCPCreate(test_net, test_mask);

    
    actual_count = DHCPCountFree(test_dhcp);

    /* check if free IP count is correct */
    if(actual_count != expected_count)
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Expected count %ld, actual count %ld\n",
                               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;

    }
    
    DHCPDestroy(test_dhcp);
    
    return 0;
}

static int TestFreeAdr()
{
    ip_t actual_ip = {{0, 0, 0, 0}};
    ip_t expected_ip = {{192, 168, 1, 23}};
    uint32_t test_mask = 0xFFFFFF00; /* 255.255.255.0 */
    dhcp_t *test_dhcp = NULL;
    size_t actual_count = 0;
    size_t expected_count = 252;
    enum dhcp_status status = OK;

    /* create DHCP */
    IPStrToArr("192.168.1.0", &actual_ip);

    test_dhcp = DHCPCreate(actual_ip, test_mask);

    /* ask for some ips */
    expected_ip.ip_addr[3] = 1;
    DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);

    expected_ip.ip_addr[3] = 2;
    DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);

    expected_ip.ip_addr[3] = 3;
    DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);

    /* free one IP */
    expected_ip.ip_addr[3] = 1;
    DHCPFreeAddr(test_dhcp, expected_ip);

    actual_count = DHCPCountFree(test_dhcp);

    /* check if IP count is right */
    if(actual_count != expected_count)
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Expected count %ld, actual count %ld\n",
                               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;

    }

    /* ask for freed ip again */
    expected_ip.ip_addr[3] = 1;
    status = DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);

    /* check if reserved IP is the one that was requested */
    if(!IPIsSameIP(expected_ip, actual_ip) || ADDR_TAKEN == status)
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Got Unexpected IP\n", RED, __LINE__ - 3);
        printf("\tExcpected:");
        DisplayIP(expected_ip);
        printf("\tGot:");
        DisplayIP(actual_ip);

        return 1;
    }


    DHCPDestroy(test_dhcp);
    
    return 0;
}

static int TestAddToFull()
{
    ip_t test_net = {{0, 0, 0, 0}};
    ip_t actual_ip = {{0, 0, 0, 0}};
    ip_t expected_ip = {{192, 168, 1, 0}};
    uint32_t test_mask = 0xFFFFFF00; /* 255.255.255.0 */
    dhcp_t *test_dhcp = NULL;
    size_t actual_count = 0;
    size_t i = 0;
    size_t expected_count = 0;
    enum dhcp_status status = OK;

    /* create DHCP */
    IPStrToArr("192.168.1.0", &test_net);

    test_dhcp = DHCPCreate(test_net, test_mask);

    for(; 256 > i; ++i)
    {
        expected_ip.ip_addr[3] = i;
        DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);
    }

    actual_count = DHCPCountFree(test_dhcp);

    /* check if IP count is right */
    if(actual_count != expected_count)
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Expected count %ld, actual count %ld\n",
                               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;

    }

    /* ask for some ip */
    status = DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);

    /* check if a new address was added to full DHCP */
    if(NO_FREE_ADDR != status)
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Got new IP with full tree\n", RED, __LINE__ - 3);
        printf("\tGot: ");
        DisplayIP(actual_ip);
        return 1;
    }

    DHCPDestroy(test_dhcp);
    
    return 0;
}

static int TestGetZero()
{
    ip_t test_net = {{192, 168, 1, 0}};
    ip_t actual_ip = {{192, 168, 1, 0}};
    ip_t expected_ip = {{192, 168, 1, 1}};
    uint32_t test_mask = 0xFFFFFF00; /* 255.255.255.0 */
    dhcp_t *test_dhcp = NULL;

    /* create DHCP */
    IPStrToArr("192.168.1.0", &test_net);

    test_dhcp = DHCPCreate(test_net, test_mask);

    /* ask for ip zero - should allocate 192.168.1.1 */
    DHCPGetAddr(test_dhcp, actual_ip, &actual_ip);

    /* check if reserved IP matches expected value */
    if(!IPIsSameIP(expected_ip, actual_ip))
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Got Unexpected IP\n", RED, __LINE__ - 3);
        printf("\tExcpected: ");
        DisplayIP(expected_ip);
        printf("\tGot: ");
        DisplayIP(actual_ip);

        return 1;
    }
    
    DHCPDestroy(test_dhcp);
    
    return 0;
}

static int TestGet()
{
    ip_t test_net = {{0, 0, 0, 0}};
    ip_t actual_ip = {{0, 0, 0, 0}};
    ip_t expected_ip = {{192, 168, 1, 23}};
    ip_t expected_ip2 = {{192, 168, 1, 22}};
    uint32_t test_mask = 0xFFFFFF00; /* 255.255.255.0 */
    dhcp_t *test_dhcp = NULL;
    size_t actual_count = 0;
    size_t expected_count = 253;

    /* create DHCP */
    IPStrToArr("192.168.1.0", &test_net);

    test_dhcp = DHCPCreate(test_net, test_mask);

    /* ask for some ip */
    DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);

    actual_count = DHCPCountFree(test_dhcp);
    
    /* check if IP count is right */
    if(actual_count != expected_count)
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Expected count %ld, actual count %ld\n",
                               RED, __LINE__ - 4, expected_count, actual_count);
        return 1;

    }

    /* check if reserved IP is the one that was requested */
    if(!IPIsSameIP(expected_ip, actual_ip))
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Got Unexpected IP\n", RED, __LINE__ - 3);
        printf("\tExcpected:");
        DisplayIP(expected_ip);
        printf("\tGot:");
        DisplayIP(actual_ip);

        return 1;
    }
    
    /* ask for some ip again */
    DHCPGetAddr(test_dhcp, expected_ip, &actual_ip);

    /* check if reserved IP is very close to what was requested */
    if(!IPIsSameIP(expected_ip2, actual_ip))
    {
        DHCPDestroy(test_dhcp);
        printf("\t%sLine %d - Got Unexpected IP\n", RED, __LINE__ - 3);
        printf("\tExcpected: ");
        DisplayIP(expected_ip2);
        printf("\tGot: ");
        DisplayIP(actual_ip);

        return 1;
    }

    DHCPDestroy(test_dhcp);
    
    return 0;
}

int main()
{

    RUN_TEST(TestCreateDestroy);
    RUN_TEST(TestGet);
    RUN_TEST(TestAddToFull);
    RUN_TEST(TestFreeAdr);
    RUN_TEST(TestGetZero);

    return 0;
}
