# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>

# include "iolist.h"

#define SIZE NCores
#define LARGENUMBER 999999999

#define  NO_OF_PARTICLES 5000
#define  MAX_ITERATION       10000
#define ALPHA 0.1
#define BETA 0.1
#define  GAMMA 0.2
#define TAM_WIDTH_MAX 32
#define BIT_LENGTH 5

typedef struct swapsequence{
int from;
int to;
} swapseq;

typedef struct particlestructure{
int info[BIT_LENGTH *SIZE];
int lbest[BIT_LENGTH * SIZE];
long int lbesttime;
long int time_fitness;
} particle;

typedef struct {
        int corenum;
        int tam_width ;
        long int starttime ;
        long int endtime ;
} schedulerInfo ;

typedef struct 
{
	int tam_list[TAM_WIDTH_MAX];
	long int testtime_list[TAM_WIDTH_MAX];
	int no_of_tam;
} tam_testtime_information;

tam_testtime_information tam_testtime[SIZE];
schedulerInfo scheduler [ NO_OF_PARTICLES ] [ SIZE ];
particle partarray[NO_OF_PARTICLES];
particle globalbest;
schedulerInfo BestParticle [SIZE];

int TAM_INFO[]= TAM;
int globalbestIndx;
//long int temptime=LARGENUMBER;

int bin_to_decimal(int binary)
{
	//int binary = 0;
	int decimal = 0;
	int remainder;
	int power = 1;
	
	while(binary!=0)
	{
        	remainder = binary % 10;
        	decimal = decimal + remainder * power;
        	power = power * 2;
        	binary=binary/10;
    	}	
	return decimal;
}


long int bin_packing(int *particle_info, int index, int genaration)
{
	int binary;
	long int testtime[SIZE];
	int assigned_tam[SIZE];
	double area[SIZE];
	int schedule_index[SIZE];
	int no_of_core_scheduled = 0;
	int tam_index[SIZE];
	long int break_point[SIZE+2];
	int available_tam_width[SIZE+2];
	int insert_index;
	long int temp;
	double max_weightage;
	int max_index;
	for(int  i = 0; i< SIZE; i++)
		scheduler [index][i].starttime = -1;
	/*for(int i = 0; i< SIZE; i++)
	{
		tam_index[i] = (int)(particle_info[i] * tam_testtime[i].no_of_tam);
	}*/
	for(int i = 0; i< SIZE; i++)
	{
		binary = 0;
		for(int k = 0; k < BIT_LENGTH ; k++)
			binary = binary + particle_info[(i* BIT_LENGTH) + k] * pow (10,BIT_LENGTH - 1 -k);
		tam_index[i] = bin_to_decimal(binary);
	}		
	/*for(int i = 0; i< SIZE; i++)
		printf("%d\t",tam_index[i]);
	printf("\n\n\n");*/		
	for(int i = 0; i< SIZE; i++)
	{
		assigned_tam[i] = tam_testtime[i].tam_list[tam_index[i]];
		testtime[i] = tam_testtime[i].testtime_list[tam_index[i]];
		area[i] = assigned_tam[i] * testtime[i];
	}
	//printf("\n i am here\n");
	for(int i = 0; i< SIZE; i++)
			schedule_index[i] = 0;
			
	for(int i = 0; i<= SIZE+1; i++)
		available_tam_width[i] = TAM_WIDTH_MAX;
			
	break_point[0] = 0;
	for(int i = 1; i<= SIZE+1; i++)
		break_point [i] = LARGENUMBER;
	int t =0;
	
	
	while (no_of_core_scheduled < SIZE)
	{
		int flag = 1;
		while(flag ==1)
		{
			max_weightage = 0;
			//max_weightage = LARGENUMBER;
			flag = 0;
			int jj = 0;
			for (int kk = 0; kk< SIZE; kk++)
			{
				if(schedule_index[kk] == 0 )
				{
					if (available_tam_width[jj] >= assigned_tam[kk])
					{
						flag = 1;
						if(area[kk]> max_weightage)
						{
						max_weightage = area[kk];
						max_index = kk;
						}
					}
				}
			}
			if (flag == 0)
			break;
			
			
			scheduler [index][t].corenum =  max_index;
			scheduler [index][t].tam_width = assigned_tam[max_index];
			scheduler [index][t].starttime = break_point[jj];
			scheduler [index][t].endtime = break_point[jj] + testtime [max_index];	
			
			int number_of_brk = 0;
			while(break_point [number_of_brk] != LARGENUMBER)
				number_of_brk++;
			break_point[number_of_brk] = scheduler [index][t].endtime;
			
			for (int i = 0; i<=(SIZE); i++)
				for (int j = 0; j<=(SIZE-i); j++)
				{
					if(break_point[j]> break_point[j+1])
					{
						temp = break_point[j];
						break_point[j] = break_point[j+1];
						break_point[j+1] = temp;
					}
				}
			for (int i = 0; i<= SIZE+1; i++)
			{
				if (break_point[i] == scheduler[index][t].endtime  )
				{
					insert_index = i;
					//break;
				}	
			}
			for (int i = number_of_brk; i>= insert_index; i--)
				available_tam_width[i] = available_tam_width[i-1];	
			int a = jj;
			while(break_point[a]< scheduler[index][t].endtime )
			{		
				available_tam_width[a] = available_tam_width[a] - assigned_tam[max_index];
				a++;
			}	
			schedule_index[scheduler [index][t].corenum] = 1;
			no_of_core_scheduled++;	
			t++;
		}
		
		
		int latest_schedule_cores[SIZE];
		long int latest_schedule_core_testtime[SIZE];
		int latest_schedule_core_tam[SIZE];	
		int k = 0;
		int no_of_latest_scheduled_core = 0;
		int temp1;
		long int temp2;
		for(int i = 0; i< SIZE; i++)
		{
			//if(schedule_index[scheduler [index][i].corenum] == 1)
			if (scheduler [index][i].starttime == break_point[0] )
			{
				latest_schedule_cores[k] = scheduler [index][i].corenum;
				latest_schedule_core_testtime[k] = scheduler [index][i].endtime;
				latest_schedule_core_tam[k] = scheduler [index][i].tam_width;
				k++;
				no_of_latest_scheduled_core++;	
			}		
		}
		for(int i = 0; i< no_of_latest_scheduled_core-1; i++)
		{
			for(int j = 0 ; j< no_of_latest_scheduled_core-i-1; j++)
			{
				if(latest_schedule_core_testtime[j] <latest_schedule_core_testtime[j+1])
				{
					temp1 = latest_schedule_cores[j];
					temp2 = latest_schedule_core_testtime[j];
					latest_schedule_cores[j] = latest_schedule_cores[j+1];
					latest_schedule_core_testtime[j] = latest_schedule_core_testtime[j+1];
					latest_schedule_cores[j+1] = temp1;
					latest_schedule_core_testtime[j+1] = temp2;
				}
			}
		}
	//if(genaration == MAX_ITERATION -1)
	//{
		int core_check = 0;
		if(available_tam_width[0] > 0)
		{	
			int kk = 0;
			while((no_of_latest_scheduled_core - core_check) > 0 && available_tam_width[0] > 0)
			{		
				int extra_tam = available_tam_width[0];
				int break_count = 0;
				int core_number;
				int core_index_in_scheduler;
				int position_in_break_point;
				int current_tam_index, next_tam_index;
				int current_tam, next_tam;
				long int current_testtime, next_testtime;
				while(break_point [break_count] != LARGENUMBER)
					break_count++;
				for (int i = 0; i< break_count; i++)
				{
					if(break_point[i] == latest_schedule_core_testtime[kk])
					{
						position_in_break_point = i;
						break;
					}	
				}
				/*printf("\n\n+++++++++++++++++++++++++++++++++++++++++++++\n\n");
				for(int i = 0; i<= SIZE+1; i++)
					printf("%ld\t", break_point[i]);
				printf("\n\n");
				for(int i = 0; i<= SIZE+1; i++)
					printf("%d\t", available_tam_width[i]);
				printf("\n\n");
				*/	
				for (int i = 0; i< SIZE; i++)
				{
					if(scheduler[index][i].corenum == latest_schedule_cores[kk])
					{
						core_number = latest_schedule_cores[kk];
						core_index_in_scheduler = i;
						break;
					}
				}
				for(int i = 0; i< SIZE; i++)
				{
					if(i == core_number)
					{
						current_tam_index = tam_index[i];
						break;
					}
				}
				current_tam = tam_testtime[core_number]. tam_list[current_tam_index];
				current_testtime = tam_testtime[core_number]. testtime_list[current_tam_index];
				next_tam_index = current_tam_index + 1;
				int check = 0;
				while(next_tam_index < tam_testtime[core_number].no_of_tam && tam_testtime[core_number].tam_list[next_tam_index]- current_tam <= extra_tam)
				{
					check = 1;
					next_tam_index ++;
				}
				if(check == 1)
				{
					tam_index[core_number] = next_tam_index-1;
					next_tam = tam_testtime[core_number].tam_list[next_tam_index-1];
					next_testtime = tam_testtime[core_number].testtime_list[next_tam_index-1];
					for(int i = position_in_break_point; i< SIZE+1; i++)
						break_point[i] = break_point[i+1];
					for(int i = 0; i< (position_in_break_point); i++)
					{
						available_tam_width[i] = available_tam_width[i]+ current_tam;
					}
					for(int i = position_in_break_point; i< SIZE+1; i++)
						available_tam_width[i] = available_tam_width[i+1];
					/*printf("\n\n********************************************\n\n");
					for(int i = 0; i<= SIZE+1; i++)
						printf("%ld\t", break_point[i]);
					printf("\n\n");
					for(int i = 0; i<= SIZE+1; i++)
						printf("%d\t", available_tam_width[i]);
					printf("\n\n");	*/
					scheduler [index][core_index_in_scheduler].tam_width = next_tam;
					scheduler [index][core_index_in_scheduler].endtime = break_point[0] + next_testtime;	
					int number_of_brk1 = 0;
					while(break_point [number_of_brk1] != LARGENUMBER)
						number_of_brk1++;
					break_point[number_of_brk1] = scheduler [index][core_index_in_scheduler].endtime;
					long int temp3;
					for (int i = 0; i<=(SIZE); i++)
						for (int j = 0; j<=(SIZE-i); j++)
						{
							if(break_point[j]> break_point[j+1])
							{
								temp3 = break_point[j];
								break_point[j] = break_point[j+1];
								break_point[j+1] = temp3;
							}
						}	
					for (int i = 0; i<= SIZE+1; i++)
					{
						if (break_point[i] == scheduler [index][core_index_in_scheduler].endtime )
						{
							insert_index = i;
							//break;
						}	
					}	
					for (int i = number_of_brk1; i>=insert_index; i--)
						available_tam_width[i] = available_tam_width[i-1];
					int a = 0;
					while(break_point[a]< scheduler [index][core_index_in_scheduler].endtime )
					{				
						available_tam_width[a] = available_tam_width[a] - next_tam;
						a++;
					}	
				}
				/*printf("\nafter\n");
				for(int i = 0; i<= SIZE+1; i++)
					printf("%ld\t", break_point[i]);
				printf("\n\n");
				for(int i = 0; i<= SIZE+1; i++)
					printf("%d\t", available_tam_width[i]);
				printf("\n\n");*/
				core_check++;
				kk++;
			}
		}
	//}	
		for (int i =0; i< SIZE+1; i++)
		{
			break_point[i] = break_point[i+1];
			available_tam_width[i] = available_tam_width[i+1];
		}
		break_point[SIZE+1] = LARGENUMBER;
		available_tam_width[SIZE+1] = TAM_WIDTH_MAX;	
	}
	
	schedulerInfo tmp;
        for ( int i = 0 ; i < SIZE ; i ++ )
        {
                for ( int j = 0 ; j < SIZE-1-i ; j ++ ) 
                {
                        if ( scheduler [index][j+1].starttime <  scheduler [index][j].starttime ) 
                        {
                                tmp.corenum = scheduler [index][j+1]. corenum ;
                                tmp.tam_width = scheduler [index][j+1]. tam_width ;
                                tmp.starttime = scheduler [index][j+1]. starttime;
                                tmp.endtime = scheduler [index][j+1]. endtime;
                                
                                scheduler [index][j+1]. corenum = scheduler [index][j]. corenum ;
                                scheduler [index][j+1]. tam_width = scheduler [index][j]. tam_width ;
                                scheduler [index][j+1]. starttime = scheduler [index][j]. starttime ;
                                scheduler [index][j+1]. endtime = scheduler [index][j]. endtime ;

                                scheduler [index][j]. corenum = tmp.corenum ;
                                scheduler [index][j]. tam_width = tmp.tam_width ;
                                scheduler [index][j]. starttime = tmp.starttime ;
                                scheduler [index][j]. endtime = tmp.endtime ;
                                
                        }
                }
        }
	
	/*int new_tam_index[SIZE];
	for(int i = 0; i< SIZE; i++)
	{
		for(int j = 0; j< tam_testtime[scheduler [index][i].corenum].no_of_tam; j++)
		{
			if(scheduler [index][i].tam_width == tam_testtime[scheduler [index][i].corenum].tam_list[j])
			{
				new_tam_index[scheduler [index][i].corenum] = j;
			}
		}
	}
	
	for(int i = 0; i< SIZE; i++)
	{
		particle_info[i] = (double)new_tam_index[i] / (double)tam_testtime[i].no_of_tam;
	}
	*/
	/*int new_tam;
	for(int i = 0; i< SIZE; i++)
	{
		new_tam = tam_testtime[i]. tam_list[tam_index[i]];
		printf("\nnew tam %d\n", new_tam);
		//printf()
		int ll = BIT_LENGTH -1;
		while(new_tam != 0)
		{
			printf("\n *****\n");
			particle_info[i*BIT_LENGTH + ll] = new_tam % 2;
			new_tam = new_tam / 2;
			ll--;
		}
		
	}
	printf("\n++++\n");*/
	int flag = 0;
	int kk;
	for ( kk = 0; kk<= SIZE+1; kk++)
		if(break_point[kk] == LARGENUMBER)
		{	
			flag = 1;
			break;
		}
	if (flag ==1)
	{
	
	if (break_point[kk-1] == 64)
	{	
		printf("\n********%ld\n",break_point[kk-1]);
		for(int i = 0; i<= SIZE+1; i++)
					printf("%ld\t", break_point[i]);
				printf("\n\n");
				for(int i = 0; i<= SIZE+1; i++)
					printf("%d\t", available_tam_width[i]);
				printf("\n\n");
	exit(0);
	}
	return(break_point[kk-1] );
	
	}
	else
	{
	
	if (break_point[kk] == 64)
	{	
		printf("\n++++++%ld\n",break_point[kk]);
		for(int i = 0; i<= SIZE+1; i++)
					printf("%ld\t", break_point[i]);
				printf("\n\n");
				for(int i = 0; i<= SIZE+1; i++)
					printf("%d\t", available_tam_width[i]);
				printf("\n\n");
	exit(0);
	}
	
	return (break_point[kk]);
	
	}
	
				
}				




/*void swaptamindex(double *a, double *b, double *c, double prob1, double prob2) //here prob is ALPHA-probal with which lbest is updated
{
	double rand1, rand2, cost;
	for (int i = 0; i< SIZE;)
	{
		rand1 = drand48();
		rand2 = drand48();
		cost = (prob1 * rand1 * (b[i] - a [i])) + (prob2 * rand2 * (c[i] - a [i]));
		a[i] = a[i]+ cost;
		if ((a[i]) >= 0 && a[i] < 1)
			i++;	
	}
}
*/
void swapparticleinfo(int *a, int *b, double prob)
{
	int binary;
	int decimal;
	for(int i = 0;i < (SIZE);)
	{
		//if(drand48() <= prob)
			for(int j = (i* BIT_LENGTH); j < ((i* BIT_LENGTH) + BIT_LENGTH); j++)	
				if(drand48() <= prob)
					a[j] = b[j];
		binary = 0;	
		for(int k = 0; k < BIT_LENGTH ; k++)
			binary = binary + a[(i* BIT_LENGTH) + k] * pow (10,BIT_LENGTH - 1 -k);
		decimal	= bin_to_decimal(binary);
		if(decimal< tam_testtime[i]. no_of_tam)
			i++;	
	}
}

void initialiseparticle(particle *object)
{
	int binary;
	int decimal;
	object->lbesttime = LARGENUMBER;
	for(int i = 0;i < (SIZE);)
	{
		
		for(int j = (i* BIT_LENGTH); j < ((i* BIT_LENGTH) + BIT_LENGTH); j++)	
			object->info[j]=rand()% 2;
		binary = 0;	
		for(int k = 0; k < BIT_LENGTH ; k++)
			binary = binary + object->info[(i* BIT_LENGTH) + k] * pow (10,BIT_LENGTH - 1 -k);
		decimal	= bin_to_decimal(binary);
		if(decimal< tam_testtime[i]. no_of_tam)
			i++;	
	}
}

int particle_swarm_optimization()
{
	
	time_t seconds;
	time( & seconds ) ;
	long int gencount=0;
	int temp=0;
	long int temptime=LARGENUMBER;
	long int lasttesttime = LARGENUMBER;
	int samegencount=0;
	
	for (int i=0;i<( NO_OF_PARTICLES  ); ){
    		initialiseparticle(&(partarray[i]));
		for ( int l = 0 ; l < i ; l ++ ) {
			int flag = 0 ;
			for ( int t = 0 ; t < BIT_LENGTH *(SIZE) && flag == 0; t ++ ) {
				if ( partarray [ l ] . info [ t ] != partarray [ i ] . info [ t ] ) {
					flag = 1 ;
					
				}
			}
			if ( flag == 0 ) break ;
			if ( l == i - 1 ) {
				i ++ ;
				break ;
			}
		}
		if ( i == 0 ) i ++;
		
	}
	
	/*for (int i=0;i<NO_OF_PARTICLES;i++)
	for(int kkk = 0; kkk< SIZE; kkk++)
        	printf("%lf\t",partarray[i].info[kkk]);
        printf("\n\n");*/
	int generation = 0;
	for (int i=0;i<NO_OF_PARTICLES;i++)
    	{
    		partarray[i].time_fitness = bin_packing(partarray[i].info,i,generation);  //testtime returns maxtime for scheduling process
    		for (int ii=0;ii< BIT_LENGTH * SIZE;ii++)
        	{
        	        partarray[i].lbest[ii]=partarray[i].info[ii];
       	 	}
    		partarray[i].lbesttime=partarray[i].time_fitness;
    	}
    	
    	int flag = 0;
    	for (int i=0;i<NO_OF_PARTICLES;i++)
    	{
      	 	if(partarray[i].time_fitness<temptime)
         	{
                	//Make it the globalbest
                	temp=i;
                	globalbestIndx = i;
                	temptime=partarray[i].time_fitness;
                	flag = 1;
            	}
    	}
    	//printf("global best********%d\n",globalbestIndx);
   	 if(flag == 1)
   	 {
    		globalbest.time_fitness=temptime;
    		// Copy info of the best particle to the global best particle
    		for (int i=0;i< BIT_LENGTH * SIZE;i++)
    			globalbest.info[i]=partarray[globalbestIndx].info[i];
    		for ( int pp = 0 ; pp < SIZE ; pp ++ )
        	{
       			BestParticle [ pp ] .corenum = scheduler [globalbestIndx][pp]. corenum;
          		BestParticle [ pp ] .tam_width = scheduler [globalbestIndx][pp]. tam_width;
               		BestParticle [ pp ] .starttime = scheduler [globalbestIndx][pp]. starttime;
                	BestParticle [ pp ] .endtime = scheduler [globalbestIndx][pp]. endtime;
                	//printf ("{ %d, %d, %d, %d }\n", scheduler [globalbestIndx][pp]. io, scheduler [globalbestIndx][pp]. corenum, scheduler [globalbestIndx][pp]. starttime, scheduler [globalbestIndx][pp]. endtime);
        	}
    	}
    	int count;
    	while(gencount<MAX_ITERATION)
    	{
        	
        	for(int i=0;i<NO_OF_PARTICLES;i++)
        	{
            	   	 /*for(int kkk = 0; kkk< SIZE; kkk++)
            	   	 	printf("%lf\t",partarray[i].info[kkk]);
            	   	 printf("\n\n");*/
            	   	 //swaptamindex(partarray[i].info,partarray[i].lbest,globalbest.info, ALPHA,BETA); //paticle's IOpair part is updated to pbest  with prob APLHA	
            		swapparticleinfo(partarray[i].info,partarray[i].lbest,ALPHA);
            		swapparticleinfo(partarray[i].info,globalbest.info,BETA);
            		/*for(int kkk = 0; kkk< SIZE; kkk++)
            	   	 	printf("%lf\t",partarray[i].info[kkk]);
            	   	 printf("\n\n");*/
            		//swapiopair(partarray[i].info,globalbest.info, BETA);     //paticle's IOpair part is updated to gbest  with prob BETA
    			
    			partarray[i].time_fitness = bin_packing(partarray[i].info,i,gencount);
    			if((partarray[i].time_fitness)<=(partarray[i].lbesttime))
            		{    
                		for (int ii=0;ii< BIT_LENGTH * SIZE;ii++)
                		{
                			partarray[i].lbest[ii]=partarray[i].info[ii];
                		}
                		partarray[i].lbesttime=partarray[i].time_fitness;
            		}
               }
               	int flag = 0;
        	for (int i=0;i<NO_OF_PARTICLES;i++)
        	{
           		 if(partarray[i].time_fitness<temptime) //temptime is holding globalbest time for a generation.
               		 {
                    		//Make it the globalbest
                   		temp=i;
                    		globalbestIndx = i;
                    		temptime=partarray[i].time_fitness;
                    		flag = 1;
                    
                	}
        	}
        	//printf("global best********%d\n",globalbestIndx);
        	if (flag == 1)
   		{
      			globalbest.time_fitness=temptime;
        		for (int i=0;i< BIT_LENGTH * SIZE;i++)
        			globalbest.info[i]=partarray[globalbestIndx].info[i];
       			for ( int pp = 0 ; pp < SIZE ; pp ++ )
       			{
				BestParticle [ pp ] .corenum = scheduler [globalbestIndx][pp]. corenum;
               		 	BestParticle [ pp ] .tam_width = scheduler [globalbestIndx][pp]. tam_width;
                		BestParticle [ pp ] .starttime = scheduler [globalbestIndx][pp]. starttime;
                		BestParticle [ pp ] .endtime = scheduler [globalbestIndx][pp]. endtime;
                		//printf ("{ %d, %d, %d, %d }\n", scheduler [globalbestIndx][pp]. io, scheduler [globalbestIndx][pp]. corenum, scheduler [globalbestIndx][pp]. starttime, scheduler [globalbestIndx][pp]. endtime);
                	}
   		}	
   		
        	for (int i=0;i<NO_OF_PARTICLES;i++)
        	{
           		if(drand48()< 0.2)
           		{
           			for(int jj = 0; jj< SIZE; jj++)
           			{
           				if(drand48()< 0.5)
           				{
           					int temp_store[BIT_LENGTH];
           					for(int ll = 0 ; ll< BIT_LENGTH; ll++)
           						temp_store[ll] = partarray[i]. info[jj * BIT_LENGTH + ll];
           					int yy = (int) rand();
           					yy = yy % BIT_LENGTH;
           					int zz = (int) rand();
           					zz = zz % BIT_LENGTH;
           					/*if(partarray[i]. info[jj * BIT_LENGTH + yy] == 0)
           						partarray[i]. info[jj * BIT_LENGTH + yy] = 1;
           					else
           						partarray[i]. info[jj * BIT_LENGTH + yy] = 0;
           					*/
           					if(temp_store[yy] == 0)
           						temp_store[yy] = 1;
           					else
           						temp_store[yy] = 0;
           					if(temp_store[zz] == 0)
           						temp_store[zz] = 1;
           					else
           						temp_store[zz] = 0;		
           					int binary = 0;
           					int decimal;	
						for(int k = 0; k < BIT_LENGTH ; k++)
							//binary = binary + partarray[i]. info[(jj* BIT_LENGTH) + k] * pow (10,BIT_LENGTH - 1 -k);
							binary = binary + temp_store[k] * pow(10, BIT_LENGTH - 1 - k);
						decimal	= bin_to_decimal(binary);
						if(decimal< tam_testtime[i]. no_of_tam)
						{	
							if(partarray[i]. info[jj * BIT_LENGTH + yy] == 0)
           							partarray[i]. info[jj * BIT_LENGTH + yy] = 1;
           						else
           							partarray[i]. info[jj * BIT_LENGTH + yy] = 0;
           						if(partarray[i]. info[jj * BIT_LENGTH + zz] == 0)
           							partarray[i]. info[jj * BIT_LENGTH + zz] = 1;
           						else
           							partarray[i]. info[jj * BIT_LENGTH + zz] = 0;	
           					}	
           				}
           			}
           		}
        	}
   		gencount++;
        	if (temptime == lasttesttime)
   		{
   			samegencount++;
   			printf("\nlasttime is %ld for gen %ld",lasttesttime, gencount);
   			printf("\n\n the semgencount value is %d", samegencount);
   		}
   		else
   		{	
   			samegencount = 0;
   			lasttesttime= temptime;
   		}	
   		/*if (samegencount == 150)
   			break;*/	
   	}
   	printf("\ngencount is %ld\n", gencount);

    	return temptime;		
}









int main(int argc, char *argv [ ])
{
	long int ppp=1111990911;
	
	
	time_t seconds;
	time( & seconds ) ;
	srand ( seconds ) ;
	srand48 ( seconds ) ; //srand generates a sequence of random numbers depending upon the seed value
	FILE *p1;

	p1 = fopen ( argv [ 1 ],"w" ) ;
	int NoIteration = atoi ( argv [ 2 ] ) ;
        //TAM_WIDTH_MAX = atoi ( argv [ 3 ] ) ;
        int iteration;

	long int t,mint;
	
	FILE *tamread;
	tamread = fopen("tam_testtime.txt","r");
	for(int ii = 0; ii< SIZE; ii++)
		tam_testtime[ii].no_of_tam =  TAM_INFO[ii];
	for(int ii = 0; ii< SIZE; ii++)
	{
		for(int jj = 0; jj< tam_testtime[ii].no_of_tam; jj++)
		{
			fscanf(tamread,"%d\t%ld", &tam_testtime[ii].tam_list[jj], &tam_testtime[ii].testtime_list[jj]); 
		}
	}
	fclose (tamread);
	/*temp_tam_index[0] = TAM_INFO[0];
	for(int i = 1; i< SIZE; i++)
	{
		temp_tam_index[i] = temp_tam_index[i-1]+TAM_INFO[i];
	}
	*/
	
	
	for(iteration=0;iteration < NoIteration;iteration++)
	{
		globalbestIndx  = 0;
		
		mint=particle_swarm_optimization();
		printf ("Globalbestindx: %d\n",globalbestIndx);
		/*for ( int pp = 0 ; pp < SIZE ; pp ++ ){
                        BestParticle [ pp ] .tam_width = scheduler [globalbestIndx][pp]. tam_width;
                        BestParticle [ pp ] .corenum = scheduler [globalbestIndx][pp]. corenum;
                        BestParticle [ pp ] .starttime = scheduler [globalbestIndx][pp]. starttime;
                        BestParticle [ pp ] .endtime = scheduler [globalbestIndx][pp]. endtime;
                }*/
		for(int i=1;i<10;i++)
		{
    			printf("RunForrestRun: %d\n",i);
    				globalbestIndx = 0;
    			
    			t=particle_swarm_optimization();
    			/*for ( int ii = 0 ; ii < SIZE ; ii ++ ) {
                printf ("{ %d, %d, %ld, %ld }\n", scheduler [globalbestIndx][ii].corenum, scheduler [globalbestIndx][ii].tam_width, scheduler [globalbestIndx][ii]. starttime, scheduler [globalbestIndx][ii]. endtime);
        }*/

			if ( t < mint ) {
				mint = t ;
				/*for ( int pp = 0 ; pp < SIZE ; pp ++ ){
                                        BestParticle [ pp ] .corenum = scheduler [globalbestIndx][pp]. corenum;
                                        BestParticle [ pp ] .tam_width = scheduler [globalbestIndx][pp]. tam_width;
                                        BestParticle [ pp ] .starttime = scheduler [globalbestIndx][pp]. starttime;
                                        BestParticle [ pp ] .endtime = scheduler [globalbestIndx][pp]. endtime;
                                }*/
			}
			
			printf("%ld \n",t);
		}
	
		fprintf(p1,"%ld\n",mint);
                printf("%ld\n",mint);
		
	}
        for ( int i = 0 ; i < SIZE ; i ++ ) {
                printf ("{ %d, %d, %ld, %ld }\n", BestParticle[i].corenum, BestParticle[i].tam_width, BestParticle[i]. starttime, BestParticle[i]. endtime);
        }
	
	

    return 0;
}

