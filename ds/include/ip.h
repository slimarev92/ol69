/****************************************
* DS:          IP ADDRESS (HEADER)      *
* CODE BY:     SASHA LIMAREV            *   
* REVIEWED BY: ROI KRAMER               *                    
*****************************************/

#ifndef IP_SLIMAREV
#define IP_SLIMAREV

#include <stdint.h> /* uint32_t */

#ifndef OL69_DHCP_H

#define IPV (4) /* implementation of 4 byte IP address space */

typedef struct
{
    unsigned char ip_addr[IPV];
} ip_t;

#endif /* OL69_DHCP_H */

/* Receives ip struct (ip), converts it to readable string, which is stored 
at the address provided by the user (str) */
void IPArrToStr(ip_t ip, const char *str);

/* Receives ip address in string form (str), converts it to an ip struct, which 
is stored at the address provided by the user (res) */
int IPStrToArr(const char *str, ip_t *res);

/* returns 1 if ip1 and ip2 represent the same ip address, otherwise
returns 0 */
int IPIsSameIP(const ip_t ip1, const ip_t ip2);

/* converts an ip struct (ip) to the corresponding 32 bit integer, taking 
the system's endianness into account */
uint32_t IPArrToNum(const ip_t ip);

/* converts a 32 bit number representing an ip address to an ip struct, 
taking the system's endianness into account */
ip_t IPNumToArr(uint32_t num);

/* receives a 32 bit number representing an IP address, prints it in 
binary form */
void DisplayBits(uint32_t ip);

/* prints ip in this format XXX.XXX.XXX.XXX */
void DisplayIP(ip_t ip);

#endif /* IP_SLIMAREV */