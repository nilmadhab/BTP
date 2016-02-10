#include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>

#define LARGE_NUMBER 99999999
#define SMALL_NUMBER -1
#define TAM_WIDTH_RANGE 300
#define SCAN_CHAIN_RANGE 10000


void assign (int wrapper_sc_chn[TAM_WIDTH_RANGE], int length);
void design_wrapper(int module_no, int TAM_WIDTH_MAX);


void design_wrapper(int module_no, int TAM_WIDTH_MAX)
{
long int input, output, bidir, no_of_sc_chn, total_input, total_output,temp, pattern;
int sc_chn[SCAN_CHAIN_RANGE], wrapper_sc_chn[TAM_WIDTH_RANGE], arr [10000];
int next_tam_index;
long int testtime;
long int wrapper_max;
long int wrapper_min;
FILE *read_input, *write_output;
read_input = fopen("t512505.soc", "r");
	for(int i = 0; i< 5 ; i++)
		fscanf(read_input,"%[^\n]\n",arr);
	for(int i = 0; i< (3 * (module_no-1)) ; i++)
		fscanf(read_input,"%[^\n]\n",arr);	
	for (int j = 0; j< 5; j++)
		fscanf(read_input,"%s",arr);
	fscanf(read_input,"%ld%s%ld%s%ld%s%ld%s",&input,arr,&output,arr,&bidir,arr,&no_of_sc_chn,arr);
	for (int i = 0; i< no_of_sc_chn; i++ )
		fscanf(read_input,"%d", &sc_chn[i]);
	
	for (int j = 0; j< 13; j++)
		fscanf(read_input,"%s",arr);
	fscanf(read_input,"%ld",&pattern);
	//printf("%ld\t%s....\n\n\n\n", pattern,arr);
fclose(read_input);	
char result_file [100]="64/balanced/full/wrapper_design_t512505_module_no_";
char r[10];
sprintf (r, "%d",module_no);
strcat (result_file, r);
strcat (result_file, ".txt");
write_output = fopen (result_file,"w");	
	total_input = input;
	total_output = output; 	
	for (int i = 0; i< (no_of_sc_chn-1); i++)
		for (int j = 0; j< (no_of_sc_chn-i-1); j++)
			{
				if(sc_chn[j]< sc_chn[j+1])
				{
					temp = sc_chn[j];
					sc_chn[j] = sc_chn[j+1];
					sc_chn[j+1] = temp;
				}
			}
	
	for (int ii = 1; ii<= TAM_WIDTH_MAX; ii++)
	{	
		for(int i = 0; i< TAM_WIDTH_RANGE; i++)
			wrapper_sc_chn[i] = -2;
		
		long int input_wrapper_max = 0;
		long int output_wrapper_max = 0;	
		if (ii <= no_of_sc_chn)
		{
			for (int jj = 0; jj< ii; jj++)
			{	
				wrapper_sc_chn[jj] = sc_chn [jj];
			}
			for (int kk = ii; kk < no_of_sc_chn; kk++)
			{
				assign(wrapper_sc_chn, sc_chn[kk]);	
			}	
			for (int k = 0; k < total_input ; k++)
			{
				assign(wrapper_sc_chn, 1);	
			}
		}
		if (ii > no_of_sc_chn)
		{
			for (int jj = 0; jj< no_of_sc_chn; jj++)
			{	
				wrapper_sc_chn[jj] = sc_chn [jj];
			}
			for (int jj = no_of_sc_chn ; jj< ii; jj++)
			{	
				wrapper_sc_chn[jj] = 1;
			}
			for (int k = 0; k < (total_input-(ii-no_of_sc_chn) ) ; k++)
			{
				assign(wrapper_sc_chn, 1);	
			}
		}
		for(int jj = 0; jj < ii; jj++)
		{
			
			if(wrapper_sc_chn [jj]> input_wrapper_max)
				input_wrapper_max = wrapper_sc_chn [jj];
			
		}
	
	
	/*}	
	for (int ii = 1; ii<= TAM_WIDTH_MAX; ii++)
	{*/	
		for(int i = 0; i< TAM_WIDTH_RANGE; i++)
			wrapper_sc_chn[i] = -2;
		
			
		if (ii <= no_of_sc_chn)
		{
			for (int jj = 0; jj< ii; jj++)
			{	
				wrapper_sc_chn[jj] = sc_chn [jj];
			}
			for (int kk = ii; kk < no_of_sc_chn; kk++)
			{
				assign(wrapper_sc_chn, sc_chn[kk]);	
			}	
			for (int k = 0; k < total_output ; k++)
			{
				assign(wrapper_sc_chn, 1);	
			}
		}
		if (ii > no_of_sc_chn)
		{
			for (int jj = 0; jj< no_of_sc_chn; jj++)
			{	
				wrapper_sc_chn[jj] = sc_chn [jj];
			}
			for (int jj = no_of_sc_chn ; jj< ii; jj++)
			{	
				wrapper_sc_chn[jj] = 1;
			}
			for (int k = 0; k < (total_output-(ii-no_of_sc_chn) ) ; k++)
			{
				assign(wrapper_sc_chn, 1);	
			}
		}
		
		for(int jj = 0; jj < ii; jj++)
		{
			
			if(wrapper_sc_chn [jj]> output_wrapper_max)
				output_wrapper_max = wrapper_sc_chn [jj];
			
		}
		
			
		
			if(input_wrapper_max > output_wrapper_max)
			{	
				wrapper_min = output_wrapper_max;
				wrapper_max = input_wrapper_max;
			}
			else
			{
				wrapper_min = input_wrapper_max;
				wrapper_max = output_wrapper_max;	
			}
		
		
		testtime = (1 + wrapper_max) * pattern + wrapper_min;
		
		
		fprintf(write_output,"%d\t%ld\t%ld", ii, testtime, wrapper_max);
		fprintf(write_output,"\n");
		for(int a = 0 ; a< ii; a++)
			printf("%d\t", wrapper_sc_chn[a]);
		printf("\n\n\n");
		printf("wmax = %ld\t wmin= %ld\t testtime = %ld ii= %d \n\n", wrapper_max, wrapper_min, testtime,ii);
	}			
	
fclose(write_output);	
	
	
	
	
	
	printf("%d\t%d\t%d\t%d\t%s\n", input,output,bidir,no_of_sc_chn,arr);
	for (int i = 0; i< no_of_sc_chn; i++ )
		printf("%d\t", sc_chn[i]);
	printf("\n\n\n");	



}
void assign (int wrapper_sc_chn[TAM_WIDTH_RANGE], int length)
{
int max = SMALL_NUMBER;
int min = LARGE_NUMBER;
int min_cost = LARGE_NUMBER;
int min_index, max_index, wrapper_index, free_wrapper_index;
int cost;
	for(int ii = 0; ii< TAM_WIDTH_RANGE; ii++)
		if (wrapper_sc_chn[ii]<0)
		{
			free_wrapper_index = ii;
			break;
		}
		
	for (int ll =0; ll < free_wrapper_index; ll++)
	{
		if (wrapper_sc_chn[ll] < min)
		{
			min = wrapper_sc_chn[ll];
			min_index = ll;
		}
		if (wrapper_sc_chn[ll] > max)
		{
			max = wrapper_sc_chn[ll];
			max_index = ll;
		}		
	}
	int flag = 0;
	for (int ll =0; ll < free_wrapper_index; ll++)
	{
		cost = (max - (wrapper_sc_chn[ll] + length));
		if (cost >= 0)
		{
			flag = 1;
			if(cost < min_cost)
			{
				min_cost = cost;
				wrapper_index = ll;
			}	
		}
	}
	if (flag ==1)
		wrapper_sc_chn[ wrapper_index] = wrapper_sc_chn[ wrapper_index] + length;
	else
		wrapper_sc_chn[min_index] = wrapper_sc_chn[min_index] + length;
return;
}

int main( int argc, char *argv [ ] )
{
	time_t seconds;
	time( & seconds ) ;
	srand ( seconds ) ;	
	int TAM_WIDTH_MAX = atoi (argv[1]);
	int NO_OF_MODULE = atoi (argv [2]);
	for (int ii = 1 ; ii <= NO_OF_MODULE; ii++)
	{
		design_wrapper (ii, TAM_WIDTH_MAX);
	}

}
