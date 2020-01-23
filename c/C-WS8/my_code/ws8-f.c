/******************************
* C-WS8                       *
* CODE BY:      SASHA LIMAREV *   
* REVIEWED BY: ALEX ZILBERMAN *                          
******************************/ 

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy */
#include "ws8.h"

#define NUM_OF_CONTACTS 10

#define UNUSED(x) (void)(x);

#define szof(var) (size_t)((long)(&var + 1) - (long)&var) /* size of variable */
#define szof_t(type) (size_t)((type *)0 + 1) /* size of type */

var_holder_t g_var_arr[3]; 
contact_t g_contact_book[NUM_OF_CONTACTS];
const char g_file_name[] = ".contacts.bin";

int main(int argc, char **argv)
{ 

/* uncomment next line to test struct of functions */
/*    StructOfFuncs();*/

/* uncomment next line to test reading/writing contacts */
/*     ReadWriteContacts();
*/
    return 0;
}

void StructOfFuncs()
{
    InitStructArr();
  
    g_var_arr[0].Add(&g_var_arr[0], 2301); /* int */
    g_var_arr[0].Add(&g_var_arr[0], 99);
    g_var_arr[0].Print(g_var_arr[0].data);

    printf("----------------\n");

    g_var_arr[1].Add(&g_var_arr[1], 2301); /* float */
    g_var_arr[1].Add(&g_var_arr[1], 99);
    g_var_arr[1].Print(g_var_arr[1].data);

    printf("----------------\n");

    g_var_arr[2].Add(&g_var_arr[2], 2301); /* string */
    g_var_arr[2].Add(&g_var_arr[2], 99);
    g_var_arr[2].Print(g_var_arr[2].data);
    g_var_arr[2].Free(g_var_arr[2].data);

    printf("----------------\n");
}

void ReadWriteContacts()
{
/* put next two lines in comments to test reading from file */
/*    InitContactBook();  */
/*    WriteContactsToFile();*/

    ReadContactsFromFile();

    DisplayAllContacs();
}

/*typedef struct date_of_birth*/
/*{*/
/*    size_t day;*/
/*    size_t month;*/
/*    size_t year;*/
/*} date_of_birth_t;*/

/*typedef struct contact */
/*{*/
/*    char first_name[50];*/
/*    char last_name[50];*/
/*    date_of_birth_t dob;*/
/*    size_t ID;*/
/*} contact_t;*/

void InitContactBook()
{
    date_of_birth_t temp = {7, 2, 1992};

    strcpy(g_contact_book[0].first_name, "Sasha");
    strcpy(g_contact_book[0].last_name, "Limarev");
    g_contact_book[0].ID = 316651371;
    g_contact_book[0].dob = temp;

    strcpy(g_contact_book[1].first_name, "Alex");
    strcpy(g_contact_book[1].last_name, "Zilberman");
    g_contact_book[1].ID = 306498171;
    temp.day = 10;
    temp.month = 3;
    temp.year =  1989;
    g_contact_book[1].dob = temp;

    strcpy(g_contact_book[2].first_name, "Ben");
    strcpy(g_contact_book[2].last_name, "Izakson");
    g_contact_book[2].ID = 311464077;
    temp.day = 2;
    temp.month = 10;
    temp.year =  1993;
    g_contact_book[2].dob = temp;

    strcpy(g_contact_book[3].first_name, "Yoav");
    strcpy(g_contact_book[3].last_name, "Klein");
    g_contact_book[3].ID = 300076247;
    temp.day = 7;
    temp.month = 9;
    temp.year =  1986;
    g_contact_book[3].dob = temp;

    strcpy(g_contact_book[4].first_name, "Adam");
    strcpy(g_contact_book[4].last_name, "Blau");
    g_contact_book[4].ID = 39434352;
    temp.day = 2;
    temp.month = 9;
    temp.year =  1984;
    g_contact_book[4].dob = temp;

    strcpy(g_contact_book[5].first_name, "Eran");
    strcpy(g_contact_book[5].last_name, "Segal");
    g_contact_book[5].ID = 39374483;
    temp.day = 3;
    temp.month = 8;
    temp.year =  1984;
    g_contact_book[5].dob = temp;

    strcpy(g_contact_book[5].first_name, "Daniel");
    strcpy(g_contact_book[5].last_name, "Maizel");
    g_contact_book[5].ID = 318820131;
    temp.day = 4;
    temp.month = 1;
    temp.year =  1998;
    g_contact_book[5].dob = temp;

    strcpy(g_contact_book[6].first_name, "Andrei");
    strcpy(g_contact_book[6].last_name, "Plakhota");
    g_contact_book[6].ID = 321163552;
    temp.day = 18;
    temp.month = 10;
    temp.year =  1986;
    g_contact_book[6].dob = temp;

    strcpy(g_contact_book[7].first_name, "Moran");
    strcpy(g_contact_book[7].last_name, "Langsam");
    g_contact_book[7].ID = 203316419;
    temp.day = 3;
    temp.month = 8;
    temp.year =  1991;
    g_contact_book[7].dob = temp;

    strcpy(g_contact_book[8].first_name, "Sandra");
    strcpy(g_contact_book[8].last_name, "Hadad");
    g_contact_book[8].ID = 26529396;
    temp.day = 5;
    temp.month = 8;
    temp.year =  1986;
    g_contact_book[8].dob = temp;

    strcpy(g_contact_book[9].first_name, "Roi");
    strcpy(g_contact_book[9].last_name, "Kramer");
    g_contact_book[9].ID = 209325083;
    temp.day = 24;
    temp.month = 7;
    temp.year =  1997;
    g_contact_book[9].dob = temp;
}

void DisplayContact(contact_t person)
{
    printf("First Name: %s\n", person.first_name);
    printf("Last Name: %s\n", person.last_name);
    printf("DOB: %lu %lu %lu %lu\n", person.dob.day, 
                                 person.dob.month, person.dob.year, person.ID);
}

void DisplayAllContacs()
{
    int i = 0;

    for(; i < NUM_OF_CONTACTS; ++i)
    {
        DisplayContact(g_contact_book[i]);
        printf("-------------------------\n");
    }
}

void WriteContactsToFile()
{
    FILE *dst_file = fopen(g_file_name, "w");
    if(NULL == dst_file)
    {
        printf("ERROR - CAN'T OPEN FILE\n");
        exit(1);
    }
    
    fwrite(g_contact_book, sizeof(g_contact_book[0]), NUM_OF_CONTACTS, dst_file);

    if(0 != fclose(dst_file))
    {
        printf("ERROR WHILE CLOSING FILE\n");
    }

    printf("%s\n", g_file_name);
}

void ReadContactsFromFile()
{
    FILE *dst_file = fopen(g_file_name, "r");
    if(NULL == dst_file)
    {
        printf("ERROR - CAN'T OPEN FILE\n");
    }
    
    fread((void *) g_contact_book, sizeof(g_contact_book[0]), NUM_OF_CONTACTS
                                 ,dst_file);

    if(0 != fclose(dst_file))
    {
        printf("ERROR WHILE CLOSING FILE\n");
    }
}

void InitStructArr()
{
    float *f_ptr = (float *) &g_var_arr[1].data;
 
    g_var_arr[2].Print = PrintString;
    g_var_arr[2].Add = AddToString;
    g_var_arr[2].Free = free;
    g_var_arr[2].data = calloc(sizeof(char), 100);
    if(NULL == g_var_arr[2].data)
    {
        printf("allocation failed\n");
        return;
    }
    strcpy(g_var_arr[2].data, "Data is: ");

    g_var_arr[0].data = 0;
    g_var_arr[0].Print = PrintInt;
    g_var_arr[0].Add = AddToInt;
    g_var_arr[0].Free = DoNothing;
    
    *f_ptr = 0.23;
    g_var_arr[1].Print = PrintFloat;
    g_var_arr[1].Add = AddToFloat;
    g_var_arr[1].Free = DoNothing;
}

void PrintInt(void *data)
{
    printf("%d\n", (int) data);
}

void PrintFloat(void *data)
{
    float *f_temp = (float *) &data;

    printf("%.2f\n", *f_temp);
}

void PrintString(void *data)
{
    printf("%s\n", (char *) data);
}

void AddToInt(var_holder_t *var, int num)
{
   var->data = (void *)(num + (int) var->data);
}

void AddToFloat(var_holder_t *var, int num)
{
    float *d_ptr = (float *) &var->data;

    *d_ptr = *d_ptr + (float) num;
}

void AddToString(var_holder_t *var, int num)
{
    sprintf((char *) var->data, "%s%d", (char *) var->data, num);
}

void DoNothing(void *data)
{
    UNUSED(data);
}
