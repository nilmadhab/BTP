#include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>



void pareto_optimal(int TAM_WIDTH_MAX, int NO_OF_MODULE)
{
	long int tam_width, testtime, longest_wrapper;
	long int tam_width_prev =0; 
	long int testtime_prev = 0; 
	long int longest_wrapper_prev=0;
	FILE *read, *write;
	for(int i = 1; i<= NO_OF_MODULE; i++)
	{
		char input_file [100]="100/full/wrapper_design_p22810_module_no_";
		char output_file [100]="100/pareto/wrapper_design_p22810_module_no_";
		char r[10];
		sprintf (r, "%d",i);
		strcat (input_file, r);
		strcat (input_file, ".txt");
		strcat (output_file, r);
		strcat (output_file, ".txt");
		read = fopen(input_file,"r");
		write = fopen(output_file,"w");
		//printf("%s\n %s\n\n", input_file, output_file);
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
	
}
int main( int argc, char *argv [ ] )
{
	time_t seconds;
	time( & seconds ) ;
	srand ( seconds ) ;	
	int TAM_WIDTH_MAX = atoi (argv[1]);
	int NO_OF_MODULE = atoi (argv [2]);
	pareto_optimal(TAM_WIDTH_MAX, NO_OF_MODULE);
}
