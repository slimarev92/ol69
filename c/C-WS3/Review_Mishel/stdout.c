#include <stdio.h>  /*printf*/

int main()
{
	printf("\n***********Character********\n");
	printf("size of signed char is: %ld\n" , sizeof(signed char));
	printf("size of unsigned char is: %ld\n" , sizeof(unsigned char));
	printf("\n***********Integer**********\n");
	printf("size of int is: %ld\n" , sizeof(int));
	printf("size of short int is: %ld\n" , sizeof(short int));
	printf("size of unsigned short int is: %ld\n" , sizeof(unsigned short int));
	printf("size of unsigned int is: %ld\n" , sizeof(unsigned int));
	printf("size of unsigned long int is: %ld\n" , sizeof(unsigned long int));
	printf("size of long int is: %ld\n" , sizeof(long int));
	printf("size of long (same as long int) is: %ld\n" , sizeof(long));
	printf("size of unsigned long int is: %ld\n" , sizeof(unsigned long int));
	printf("size of float is: %ld\n" , sizeof(float));
	printf("size of double is: %ld\n" , sizeof(double));
	printf("size of long double is: %ld\n" , sizeof(long double));
	printf("\n***********Pointer**********\n");
	printf("size of Pointer is: %ld\n\n" , sizeof(int *));
	
	return 0;
}
