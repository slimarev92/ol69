/*****************************
* C-WS4                      *
* CODE BY:     SASHA LIMAREV *   
* REVIEWED BY: BEN IZAKSON   *                          
******************************/ 

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "ws4.h"

#define ESC 27 /* ascii code for ESC character */

void DoNothing()
{

}

void TPressed()
{
    printf("T pressed.\n");
}

void APressed()
{
    printf("A pressed.\n");
}

void ESCPressed()
{
    
}

void PrintLettersLUT()
{

    char next_char = 0;

    system("stty -icanon -echo");

    next_char = getchar();
    
    while(ESC != next_char)
    {
       (*GetFunction(next_char))();
        
        next_char = getchar();
    }

    system("stty icanon echo");
}

/* implemented with if/else */
void PrintLettersIf()
{
    char next_char = 0;

    system("stty -icanon -echo");

    next_char = getchar();
    
    while(ESC != next_char)
    {
        if('T' == next_char)
        {
            printf("T pressed.\n");
        }
        else if('A' == next_char)
        {
            printf("A pressed.\n");
        }
        
        next_char = getchar();
    }

    system("stty icanon echo");
}

/* implemented with switch */
void PrintLettersSwitch()
{
    char next_char = 0;

    system("stty -icanon -echo");

    next_char = getchar();
    
    while(ESC != next_char)
    {
        switch(next_char)
        {
           case 'T':
                printf("T pressed.\n");
                break;
            
            case 'A':
                printf("A pressed.\n");
                break;          
        }
          
        next_char = getchar(); 
    }

    system("stty icanon echo");
}

func_ptr_t GetFunction(char c)
{
    static const func_ptr_t LUT[256] = 
    {
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        ESCPressed, /* ESC CHAR */
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        APressed, /* A CHAR */
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        TPressed, /* T CHAR */
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing,
        DoNothing
    };

    return LUT[(int)c];
}



