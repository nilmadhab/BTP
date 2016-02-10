#include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>



void pareto_optimal(int TAM_WIDTH_MAX)
{
long int tam_width, testtime, longest_wrapper;
long int tam_width_prev =0; 
long int testtime_prev = 0; 
long int longest_wrapper_prev=0;
FILE *read, *write;
read = fopen("64/unbalanced/full/wrapper_design_p93791_module_no_5.txt","r");
write = fopen("64/unbalanced/pareto/wrapper_design_p93791_module_no_5.txt","w");
	for(int ii =0; ii< TAM_WIDTH_MAX; ii++)
	{
		fscanf(read, "%ld\t%ld\t%ld", &tam_width,&testtime, &longest_wrapper);
		if(testtime != testtime_prev)
		{
			testtime_prev = testtime;
			fprintf(write,"%ld\t%ld", tam_width,testtime);
			fprintf(write,"\n");
		}
			
	}
fclose(read);
fclose(write);	
}
int main( int argc, char *argv [ ] )
{
	time_t seconds;
	time( & seconds ) ;
	srand ( seconds ) ;	
	int TAM_WIDTH_MAX = atoi (argv[1]);
	pareto_optimal(TAM_WIDTH_MAX);
}
