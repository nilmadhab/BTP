#include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>

FILE *f1;

double unbalanced_wrapper(int pin, int input, int output, int pattern, int *tam)
{
	
	double input_pin1;
	double output_pin1;
	double input_pin2;
	double output_pin2;
	double t_in1;
	double t_out1;
	double t_in2;
	double t_out2;	
	double t_total1;
	double t_total2;
	double max1;
	double max2;
	double min1;
	double min2;
	double t_final;	
	
		if((input * pin)%(input + output)== 0)
			input_pin1 = (int)((input * pin)/(input + output));
		else
			input_pin1 = (int)((input * pin)/(input + output))+1 ;
		if(input_pin1 > 1)
			input_pin2 = input_pin1 - 1;
		else
			input_pin2 = input_pin1;
		
		output_pin1 = pin - input_pin1;
		output_pin2 = pin - input_pin2;
		if(output_pin1 < 1)
		{
			output_pin1 = output_pin2;
			input_pin1 = input_pin2;
		}
		if(((int)input % (int)input_pin1) == 0)
			t_in1 = (int)(input/input_pin1);
		else
			t_in1 = (int)(input/input_pin1) + 1 ;	
		
		if(((int)input % (int)input_pin2) == 0)
			t_in2 = (int)(input/input_pin2);
		else
			t_in2 = (int)(input/input_pin2) + 1 ;
		if(((int)output % (int)output_pin1)== 0 )	
			t_out1 =(int) (output/output_pin1);
		else
			t_out1 = (int)(output/output_pin1) + 1 ;	
		if(((int)output % (int)output_pin2)== 0 )	
			t_out2 = (int)(output/output_pin2);
		else
			t_out2 = (int)(output/output_pin2) + 1 ;
		if(t_in1 >= t_out1)
		{	
			max1 = t_in1;
			min1 = t_out1;
		}
		else
		{
			max1 = t_out1;
			min1 = t_in1;
		}
		if(t_in2 >= t_out2)
		{
			max2 = t_in2;
			min2 = t_out2;
		}
		else
		{
			max2 = t_out2;
			min2 = t_in2;
		}
		t_total1 = (1 + max1) * pattern + min1;
		t_total2 = (1 + max2) * pattern + min2;
		if (t_total1 < t_total2)
		{
			t_final = t_total1;
			if(input_pin1 >= output_pin1)
				*tam = (int)input_pin1;
			else
				*tam = (int)output_pin1;	
			
		}
		if (t_total1 > t_total2)
		{
			t_final = t_total2;
			if(input_pin2 >= output_pin2)
				*tam = (int)input_pin2;
			else
				*tam = (int)output_pin2;	
		}
		if(t_total1 == t_total2)
		{
			t_final = t_total2;
			int tam1, tam2;
			if(input_pin1 >= output_pin1)
				tam1 = (int)input_pin1;
			else
				tam1 = (int)output_pin1;
			if(input_pin2 >= output_pin2)
				tam2 = (int)input_pin2;
			else
				tam2 = (int)output_pin2;
			if(tam1 > tam2)
				*tam = tam2;
			else
				*tam = tam1;			
		}
		//printf("\n%d\t%d\t%d\t%d\t%d\t%lf\t%lf\t%lf\t%d\n", (int)input_pin1,(int)input_pin2,(int)output_pin1,(int)output_pin2, pin,t_total1,t_total2,t_final, *tam);	
		return t_final;	
		
		//fprintf(f1,"%d\t%ld\n", tam, (long int)t_final);	  			
}
int main( int argc, char *argv [ ] )
{
	time_t seconds;
	time( & seconds ) ;
	srand ( seconds ) ;	
	//int TAM_WIDTH_MAX = atoi (argv[1]);
	int PIN_MAX = atoi (argv[1]);
	int input = atoi (argv [2]);
	int output = atoi (argv [3]);
	int pattern = atoi (argv [4]);
	double testtime;
	int tam;
	//FILE *f1;
	//f1 = fopen("new_wrapper/p93791/wrapper_design_p93791_module_no_5.txt","w");
	f1 = fopen("64/unbalanced/full/wrapper_design_p93791_module_no_5.txt","w");
	for (int ii = 2 ; ii <= PIN_MAX; ii++)
	{
		testtime = unbalanced_wrapper (ii, input, output, pattern, &tam);
		fprintf(f1,"%d\t%ld\t%d\n", tam, (long int)testtime,ii);
	}
fclose(f1);
}
