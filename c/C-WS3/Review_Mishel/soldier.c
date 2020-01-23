#include <stdio.h>  /*printf*/

int main()
{
	int sold[100];
	int i, lastkiller, exit=0, dead=1, foundthekiller =0;
	for(i=0; i<100; i++) /* array initialization */
	{
		sold[i] = 1;
	}
	printf("\n");
	i=0;
	while(1 != exit) /* while exit flag is 0, first kill */
	{
		while(0 != dead) /* while dead flag is not 0 - look for the first living soldier*/
		{
			++i;
			if(99 < i) /* reset the iterator to beginning of array */
			{
				i = 0;
			}
			
			if (1 == sold[i]) /* if soldier i is alive */
			{
				if (i == lastkiller) /*if soldier i is the last killer exit the loop*/
				{
					exit=1;
					break;
				} 
				sold[i] = 0; /* kill soldier i */
				dead = 0; /* set flag dead to 0 to exit the loop */
				foundthekiller = 0;
			}
		}
			
		while(1 != foundthekiller &&
			  1 != exit) 	/* while the found the killer flag is 0 */
		{
			++i;
			if(99 < i)
			{
			i=0;
			}
			if (1 == sold[i])
			{
			foundthekiller = lastkiller;
			1 =i;
			dead = 1;
			}
		}
	}
	printf("the survivor is: tum tum tum..: %d (you should add 1 because it starts from 0)\n" , lastkiller);
	return 0;
}

