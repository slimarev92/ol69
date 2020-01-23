#include <stdio.h> /*printf*/
#include <stdlib.h>	
#include <ctype.h>
#include <assert.h> /*assert*/
void strprints(char **envp);


int BufferEnv(char *envp[])
{
	char **envp_runn = envp;
	int stringcounter=0;
	int charcounter= 0;
	char** buff;
	
	strprints(envp);
	
	while( NULL != *(envp_runn + stringcounter)) 
	{
		++stringcounter;
	}

	buff = (char**)malloc((stringcounter+1)* sizeof(char*));

	if(NULL == buff)
	{
		free(buff);
		return 0;
	}

	stringcounter =0;

	while( NULL != *(envp_runn + stringcounter))
	{
		while( '\0' != *(*(envp_runn + stringcounter) + charcounter) )
		{
			++charcounter;
			printf("%d\n" , charcounter);
		}
		*(buff+stringcounter) = (char *)malloc((charcounter + 1) * sizeof(char));

		if((NULL ==  *(buff+stringcounter)) &&
			NULL != *(envp_runn + stringcounter))
		{
			while ( 0 != stringcounter)
			{
				free(*(buff + stringcounter));
				--stringcounter;
			}
			free(buff);
			return 0;
		}
		
		charcounter = 0;
		++stringcounter;
	}
	
	charcounter = 0;
	stringcounter = 0;	
	while( NULL != *(envp_runn + stringcounter)) 
	{
		while( '\0' != *(*(envp_runn + stringcounter) + charcounter)) 
		{
			*(*(buff+stringcounter) + charcounter) =
			 tolower(*(*(envp_runn + stringcounter) + charcounter)); 
			++charcounter;	
		}
		*(*(buff+stringcounter) + charcounter) = '\0';			
		charcounter =0;
		++stringcounter;
	}
	*(buff + stringcounter) = NULL;

	strprints(buff);
	stringcounter =0;
	
	while( NULL != *(buff + stringcounter) )
	{
		free( *(buff + stringcounter) );
		++stringcounter;
	}
	free(buff);
	
	return 0;
}


void strprints(char **env1)
{
  while( NULL != *env1 )
  {
    printf("%s\n", *env1);    
    ++env1;
  }
  
}

int main(int argc, char *argv[], char *envp[])
{	
	printf("%d , %s" , argc , *argv);
	BufferEnv(envp);

    strprints(envp);

	return 0;
	
}	

