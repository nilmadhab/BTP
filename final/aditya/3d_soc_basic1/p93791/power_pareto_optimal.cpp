#include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>


int POWER_MIN = 15000;
int POWER_MAX = 26500;
int STEP = 500;
void pareto_optimal(int TAM_WIDTH_MAX)
{
	
	FILE *read, *write;
	char input_file [100]="power_die_testime_nosort.txt";
	read = fopen(input_file,"r");
	for(int ii =5; ii<=TAM_WIDTH_MAX; ii++)
	{
		long int tam_width, testtime, longest_wrapper;
		long int tam_width_prev =0; 
		long int testtime_prev = 0; 
		long int longest_wrapper_prev=0;
		char output_file [100]="power_pareto/power_tam_";
		char r[10];
		sprintf (r, "%d",ii);
		strcat (output_file, r);
		strcat (output_file, ".txt");
		write = fopen(output_file,"w");
		fscanf(read, "%ld\t", &tam_width);
		for(int i = POWER_MIN; i<= POWER_MAX; i=i+STEP)
		{
			//printf("%s\n %s\n\n", input_file, output_file);
			{
				fscanf(read, "%ld\t", &testtime);
				if(testtime != testtime_prev)
				{
					testtime_prev = testtime;
					fprintf(write,"%d\t%ld", i,testtime);
					fprintf(write,"\n");
				}
			}
		}

		fclose(write);	
	}	
	
	fclose(read);
}
int main( int argc, char *argv [ ] )
{
	time_t seconds;
	time( & seconds ) ;
	srand ( seconds ) ;	
	int TAM_WIDTH_MAX = atoi (argv[1]);
	pareto_optimal(TAM_WIDTH_MAX);
}
