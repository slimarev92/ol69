/******************************************
*                _____  __     _   ___    *
*               (  _  )(  )   / ) / _ \   *
*    |\---/|     )(_)(  )(__ / _ \\_  /   *
*    | ,_, |    (_____)(____)\___/ (_/    *
*     \_`_/-..----.                       *
*  ___/ `   ' ,""+ \                      *
* (__...'   __\    |`.___.';              *
*  (_,...'(_,.`__)/'.....+                *
*                                         *
*          Exersice: Knight's Tour        *
******************************************/
#ifndef OL69_KT_H
#define OL69_KT_H

enum status {OK = 0, M_FAIL, T_FAIL};

/* KT takes an int array and when returned status is OK */
/* it puts knighs path into the given array */
/* if first == -1 then randomly generate first */
/* return value: OK if found a correct path
				 M_FAIL for malloc failure
				 T_FAIL runtime error (time > 3 min) */
enum status KT(int *path, int first); 
enum status KTAdvanced(int *path, int first); 

#endif
