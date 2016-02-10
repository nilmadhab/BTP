# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>

FILE *io, *tamtesttime;

int create_io_tam ( int TAM_WIDTH_MAX, int NO_OF_MODULE)
{
	
	//int module_no;
	int tam;
	long int testtime;
	int total_tam = 0;
	for(int i = 1;i<= NO_OF_MODULE; i++)
	{
		FILE *read;
		char input_file [100]="../sic_level_";
		char r[10];
		sprintf (r, "%d",i);
		strcat (input_file, r);
		strcat (input_file, ".txt");
		read = fopen(input_file,"r");
		int count = 0;
		while(!feof(read))
		{
			fscanf(read,"%d\t%ld\n", &tam, &testtime);
			if(tam <= TAM_WIDTH_MAX)
			{	
				fprintf(tamtesttime,"%d\t%ld\n", tam, testtime);
				count++;
			}	
		}
		fprintf(io,"%d,",count);
		total_tam = total_tam + count;
	}
	return (total_tam);
}



int main( int argc, char *argv [ ] )
{
		
	int TAM_WIDTH_MAX = atoi (argv[1]);
	int NO_OF_MODULE = atoi (argv [3]);
	int TSV_MAX = atoi (argv [2]);
	int tamsum;
	


	io = fopen("iolist.h","w");
	tamtesttime = fopen("tam_testtime.txt", "w");
	fprintf(io,"#define TAM_WIDTH_MAX %d\n", TAM_WIDTH_MAX);
	if(TAM_WIDTH_MAX <= 16)
		fprintf(io,"#define BIT_LENGTH 4\n" );
	else if(TAM_WIDTH_MAX > 16 && TAM_WIDTH_MAX <= 32)
		fprintf(io,"#define BIT_LENGTH 5\n" );
	else if(TAM_WIDTH_MAX > 32 && TAM_WIDTH_MAX <= 64)
		fprintf(io,"#define BIT_LENGTH 6\n" );
	else if(TAM_WIDTH_MAX > 64)
		fprintf(io,"#define BIT_LENGTH 7\n" );
	fprintf(io,"#define TSV_MAX %d\n",TSV_MAX);
	fprintf(io,"#define NCores %d\n",NO_OF_MODULE);
	fprintf(io,"#define TAM {");
	tamsum = create_io_tam ( TAM_WIDTH_MAX,NO_OF_MODULE);
	fseek(io,-1, SEEK_END);
	fprintf(io,"}\n");
	fprintf(io,"#define TAMSUM %d", tamsum);
	fclose(io);
	fclose(tamtesttime);

}
