#include <iostream>
using namespace std;
// variables

#define SIZE Ncores
#define NO_OF_PARTICLES 100
#define MAX_ITERATION 100
#define ALPHA 0.1
#define BETA 0.1
#define GAMMA 0.2
#define BIT_LENGTH 6
#define NEW_PARTICLE_SIZE BIT_LENGTH*SIZE

// all structures
// we can use class too
// user defined structures
// swapseq, particle,schedulerInfo,power_testtime_information
typedef struct swapsequence
{
	int from;
	int to;
	
}swapseq;
typedef struct particlestructure
{
	int info[NEW_PARTICLE_SIZE];
	int lbest[NEW_PARTICLE_SIZE];
	long int lbesttime;
	long int time_fitness;
	
}particle;

typedef struct
{
	int corenum;
	int tam_width;
	long int starttime;
	long int endtime;
	double power;
	int tsv;
	
}schedulerInfo;

typedef struct 
{
	int power_list[POWER_MAX];
	long int testtime_list[POWER_MAX];
	int no_of_power;
	
}power_testtime_information;

typedef struct 
{
	int tam_list[TAM_WIDTH_MAX];
	long int testtime_list[TAM_WIDTH_MAX];
	power_testtime_information power_testtime_list[TAM_WIDTH_MAX];
	int no_of_tam;
	
}tam_testtime_information;

tam_testtime_information tam_testtime[SIZE];
schedulerInfo scheduler [NO_OF_PARTICLES][SIZE];
particle partarray[NO_OF_PARTICLES];
particle globalbest;
schedulerInfo BestParticle [SIZE];

double power_budget;

int TAM_INFO[] = TAM;
int globalbestIndx;


int hard_tam_width[SIZE] = {15,12,12,10,7};
long int hard_test_time[SIZE] = {1947063, 1384949, 651281, 669329, 96297};



// here comes the functions

//1. int tsv_required
// takes paramenter parallel_index array
// assigned tam array, layer_tsv array

// 2. bin packing
// takes particle_info, index, generation

// swapparticle info
// initialize particle takes particle object


// PSO

// int main

long int bin_packing(int *particle_info, int index, int generation)
{
	int binary;
	long int testtime[SIZE];
	int assigned_tam[SIZE];
	int assigned_peak_power[SIZE];
	double area[SIZE];
}

void swapparticleinfo(int *a , int *b, double prob){
	int binary;
	int decimal, power_decimal;
	int flag = 1;
	while(flag)
	{
		int tsv_count = 0;
		int temp[SIZE] = 0;
		for(int i = 0 ; i < SIZE; ){
			for(int j = (i*BIT_LENGTH) ; j < (i*BIT_LENGTH) + BIT_LENGTH; j++)
				if(drand48() <= prob)
					a[j] = b[j];
			binary = 0;
			for(int k = 0 ; k < BIT_LENGTH ; k++){
				binary += a[(i*BIT_LENGTH) + k] *pow(10, BIT_LENGTH-1-k);
			}
			decimal = bin_to_decimal(binary);


			if(drand48() <= prob){
				a[i+BIT_LENGTH*SIZE] = b[i+BIT_LENGTH*SIZE];
			}

			if(decimal < tam_testtime[i].no_of_tam && a[i+BIT_LENGTH*SIZE] < tam_testtime[i].power_testtime_list[decimal].no_of_power){
				if(tam_testtime[i].power_testtime_list[decimal].power_list[a[i+BIT_LENGTH*SIZE]] < power_budget){
					temp[i] = tam_testtime[i].tam_list[decimal];
					i++;
				}
			}


		}

		for(int i = 1; i < SIZE; i++)
		{			
			int max = 0;
			for(int ii = i; ii < SIZE; ii++)
			{
				if(temp[ii] >= max)
				{
					max = temp[ii];
				}
			}
			
			tsv_count = tsv_count + max;
		}
		if(tsv_count <= TSV_MAX)
		{
			flag = 0;
	
		}
	}
}

void initialiseparticle(particle *object){
	int binary;
	int decimal, power_decimal;
	object->lbesttime = LARGENUMBER;

	int flag = 1;
	while(flag){
		int tsv_count = 0;
		int temp[SIZE];
		for(int jj=0; jj < SIZE; jj++){
			temp[jj] = 0;
		}
		for(int i = 0 ; i < SIZE ; ){
			for(int j = (i*BIT_LENGTH); j < (i*BIT_LENGTH) ; j++)
				object->info[j] = rand()%2;
			binary = 0;
			for(int k = 0; k < BIT_LENGTH; k++){
				binary += object->info[(i*BIT_LENGTH)+k]*pow(10, BIT_LENGTH-1-k);
			decimal = bin_to_decimal(binary);


				if(decimal < tam_testtime[i].no_of_tam)
				{
					int max_power_index = tam_testtime[i].power_testtime_list[decimal].no_of_power;
					int assigned_power_index = rand()% max_power_index;

					if(tam_testtime[i].power_testtime_list[decimal].power_list[assigned_power_index] < power_budget){
						object->info[(i + (BIT_LENGTH*SIZE))] = assigned_power_index;
						temp[i] = tam_testtime[i].tam_list[decimal];
						i++;
					}
				}

			for(int i = 1; i < SIZE; i++)
			{			
				int max = 0;
				for(int ii = i; ii < SIZE; ii++)
				{
					if(temp[ii] >= max)
					{
						max = temp[ii];
					}
				}
				
				tsv_count = tsv_count + max;
			}
			if(tsv_count <= TSV_MAX)
			{
				flag = 0;
				
			}
			}
		}
	}
}

int particle_swarm_optimisation(){
	time_t seconds;
	time (& seconds);
	long int gencount = 0;
	int temp = 0;
	long int temptime = LARGENUMBER;
	long int lasttesttime = LARGENUMBER;
	int samegencount = 0;

	for (int i = 0; i < NO_OF_PARTICLES;)
	{
		initialiseparticle(&partarray[i]);

		for(int l=0; l < i ;l ++){
			int flag = 0;
			for (int t = 0; i < NEW_PARTICLE_SIZE && flag == 0; ++t)
			{
				/* code */
				if(partarray[l].info[t] != partarray[i].info[t]){
					flag = 1;
				}
			}
		}
		
	}

	int generation = 0;

	for(int i = 0 ; i < NO_OF_PARTICLES; i++){
		partarray[i].time_fitness = bin_packing(partarray[i].info,i,generation);

		for(int ii=0; ii < NEW_PARTICLE_SIZE; ii++){
			partarray[i].lbest[ii] = partarray[i].info[ii];
		}
		partarray[i].lbesttime = partarray[i].time_fitness;
	}

	int flag = 0;
	for(int i=0; i < NO_OF_PARTICLES; i++){

		if(partarray[i].time_fitness < temptime){
			temp = i;
			// make it global best
			globalbestIndx = i;
			temptime = partarray[i].time_fitness;
			flag = 1;
		}
	}

	if(flag == 1){
		globalbest.time_fitness = temptime;
		// copy info of best particle to global best particle
		for(int i = 0 l i < NEW_PARTICLE_SIZE; i++)
			globalbest.info[i] = partarray[globalbestIndx].info[i];
		for(int pp=0; pp < SIZE ; pp++){
			BestParticle[pp].corenum = scheduler[globalbestIndx][pp].corenum;
			BestParticle[pp].tam_width = scheduler[globalbestIndx][pp].tam_width;
			BestParticle[pp].starttime = scheduler[globalbestIndx][pp].starttime;
			BestParticle [ pp ] .endtime = scheduler [globalbestIndx][pp]. endtime;
            BestParticle [ pp ] .tsv = scheduler [globalbestIndx][pp]. tsv;
            BestParticle [ pp ] .power = scheduler [globalbestIndx][pp]. power;
		}

		int count;
		while(gencount < MAX_ITERATION){
			for(int i = 0; i < NO_OF_PARTICLES; i++){
				swapparticleinfo(partarray[i].info,partarray[i].lbest,ALPHA);
				swapparticleinfo(partarray[i].info,globalbest.info,BETA);

				partarray[i].time_fitness = bin_packing(partarray[i].info,i, gencount);


				if(partarray[i].time_fitness <= partarray[i].lbesttime)
				{
					for(int ii=0; ii < NEW_PARTICLE_SIZE; ii++){
						partarray[i].lbest[ii] = partarray[i].info[ii];
					}
					partarray[i].lbesttime = partarray[i].time_fitness;
				}
			}

			int flag = 0;

			for(int i = 0 ; i < NO_OF_PARTICLES ; i++){

				if(partarray[i].time_fitness  < temptime)
				{
					temp = i;
					globalbestIndx = i;
					temptime = partarray[i].time_fitness;
					flag = 1;
				}
			}


			if(flag == 1)
			{
				globalbest.time_fitness = temptime;
				for(int i = 0 ; i < NEW_PARTICLE_SIZE; i ++)
				{
					for(int pp = 0 ; pp < SIZE; pp++)
					{
						BestParticle[pp].corenum = scheduler[globalbest][pp].corenum;
						BestParticle[pp].tam_width = scheduler[globalbestIndx][pp] = tam_width;
						BestParticle [ pp ] .endtime = scheduler [globalbestIndx][pp]. endtime;
                		BestParticle [ pp ] .tsv = scheduler [globalbestIndx][pp]. tsv;
                		BestParticle [ pp ] .power = scheduler [globalbestIndx][pp]. power;
					}
				}
			}
		}
	}
}


int main(int argc, char  *argv[])
{
	/* code */
	long int ppp = 1111990911;
	time_t seconds;
	// generate sequence of random numbers
	srand(seconds);
	srand48(seconds);

	p1 = fopen( argv[1], "w");
	int NoIneration = atoi(argv[2]);
	power_budget = atoi(argv[3]);

	int iteration;
	long int t,mint;

	FILE *tamread;
	tamread = fopen("tam_testtime.txt","r");
	
	for (int i = 0; i < SIZE; ++i)
		{

			for (int j = 0; j < tam_testtime[i].no_of_tam; ++j)
			{
				fscan(tamread,"%d\t%ld",&tam_testtime[i].tam_list[j], &tam_testtime[i].testtime_list[j]);
				
			}
		}	
	fclose(tamread);
	for(int ii=0; ii < SIZE; ii++){
		for(int jj=0; jj < tam_testtime[ii].no_of_tam; jj++){
			char input_file[100] = "sic_power_level_";
			char r[20];
			sprintf(r,"%d",ii+1);
			long int power, testtime;
			sprintf (r, "/power_tam_%d",jj+5);
			strcat (input_file, r);
			strcat (input_file, ".txt");
			// printf("%s\n", input_file);

			tamread = fopen(input_file,"r");
			int count = 0;
			while(!feof(tamread)){
				fscan(tamread, "%ld\t%ld\t\n",&power,&testtime);
				tam_testtime[ii].power_testtime_list.power_list[count] = power;
				tam_testtime[ii].power_testtime_list[jj].testtime_list[count] = testtime;
				count++;				
			}
			tam_testtime[ii].power_testtime_list[jj].no_of_power=count;
		}
	}
	if(HARD_DIE_TEST)
	{
		initialiseparticle(&(partarray[0]));
		partarray[0].time_fitness = bin_packing(partarray[0].info,0,0);
		for (int i = 0; i < SIZE; ++i)
		{
			/* code */
		}

	}

	for(iteration=0; iteration < NoIneration; iteration++){
		globalbestIndx = 0;

		mint = particle_swarm_optimisation();
		for(int i=1; i<2;i++){
			globalbestIndx = 0;
			t = particle_swarm_optimisation();


			if(t < mint){
				mint = t;
			}
		}
	}

	return 0;
}




void change(int &a, int &b){
	int tmp = b;
	b = a;
	a = tmp;
}
/*int main(){
	int a = 90;
	int b = 80;
	cout << a <<endl;
	cout << b <<endl;
	change(a,b);
		//cout << a <<endl;
	cout << "after changing" << endl;
	cout << a <<endl;
		cout << b <<endl;

}*/
