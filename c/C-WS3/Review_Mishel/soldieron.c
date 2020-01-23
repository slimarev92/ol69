#include <stdio.h>  /*printf*/

int main()
{
	int sold[100];
	int alive=0, tokill=1,i;
	sold[99] = 0;
	
	for(i=0; i< 99; i++)
	{
		sold[i] = i+1;
	}
	
	i=0;
	while(tokill != alive)
	{	
		tokill=sold[i];
		alive=sold[tokill];		
		sold[i]=alive;
		sold[tokill] = 0;
		i=alive;
	}
	
	printf("%d\n", alive);
	return 0;
}


