# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>
#include <iostream>
using namespace std;
#define TAM_WIDTH_MAX 31
#define BIT_LENGTH 4
#define TSV_MAX 140
#define NCores 5
#define TAM {11,10,11,11,11}
#define TAMSUM 54

#define SIZE NCores
#define LARGENUMBER 999999999
//5000 10000
#define  NO_OF_PARTICLES 3
#define  MAX_ITERATION 3
#define ALPHA 0.1
#define BETA 0.1
#define  GAMMA 0.2
//#define TAM_WIDTH_MAX 15
//#define BIT_LENGTH 6	
//#define TSV_MAX 140





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

int tsv_required(int parallel_index[], int assigned_tam[], int layer_tsv[])
{	

	//cout << "inside tsv_required" << endl;

	/*for (int i = 0; i < SIZE; ++i)
	{
		cout << parallel_index[i] << " " << assigned_tam[i] << endl;
	}*/
	//cout << "parallel_index  and assigned_tam" << endl;
	int total_tsv = 0;
	int lowest_die_test = 0;
	if(parallel_index[0])
	{
		lowest_die_test = 1;
		for(int i = 1; i < SIZE; i++)
		{
			if(parallel_index[i])
			{
				lowest_die_test = 0;
			}
		}
	}
	if(lowest_die_test)
	{
		return 0;
	}
	for(int i = 1; i < SIZE; i++)
	{
		int temp[SIZE]; 
		int count = 0;
		for(int jj = 0; jj < SIZE; jj ++)
		{
			temp[jj] = 0;
		}
		for(int j = i; j < SIZE; j++)
		{
			int parallel_tsv_count = 0;
			for(int k = j; k < SIZE; k++)
			{
				parallel_tsv_count = parallel_tsv_count +  assigned_tam[k] * parallel_index[k];
			}
			
			if(parallel_tsv_count >= assigned_tam[j])
				temp[count] = parallel_tsv_count;
			else
				temp[count] = assigned_tam [j];
			count++;
		}
		//cout << "for i" <<" " << i <<endl;
		/*for(int jj = 0; jj < SIZE; jj ++)
		{
			cout << jj << " " << temp[jj] << endl;
		}*/
		
		int max = 0;
		for(int ii = 0; ii < SIZE; ii++)
		{
			if(temp[ii] >= max)
			{
				max = temp[ii];
			}
		}
		layer_tsv[i-1] = max;
		//printf("Layer %d : %d ",i+1, max);

		total_tsv = total_tsv + max;
		//cout << total_tsv << " total_tsv" << endl;
	}
	
	//printf("TSV used : %d\n", total_tsv);
	return total_tsv;
}

long int bin_packing(int *particle_info, int index, int genaration)
{
	int binary;
	long int testtime[SIZE];
	int assigned_tam[SIZE];
	int assigned_tsv[SIZE];
	double area[SIZE];
	int schedule_index[SIZE];
	int no_of_core_scheduled = 0;
	int tam_index[SIZE];
	long int break_point[SIZE+2];
	int available_tam_width[SIZE+2];
	int insert_index;
	long int temp;
	double max_weightage;
	double min_weightage;
	int max_index;
	int layer_tsv[SIZE-1];
	int session_tsv[SIZE-1];
	int session = 1;
	int parallel_session_index[SIZE][SIZE];
	int parallel_temp_index[SIZE];
	printf("####################Begin schedule##########################\n");

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
	for(int i = 0; i< SIZE; i++)
		printf("%d\t",tam_index[i]);
	printf("\n\n\n");		
	for(int i = 0; i< SIZE; i++)
	{
		assigned_tam[i] = tam_testtime[i].tam_list[tam_index[i]];
		testtime[i] = tam_testtime[i].testtime_list[tam_index[i]];
		area[i] = assigned_tam[i] * testtime[i];
	}
	cout << "assigned_tam and testime and area" << endl;
	for (int i = 0; i < SIZE; ++i)
	{
		cout << assigned_tam[i] << " " << testtime[i] << " " << area[i] << endl;
	}
	//printf("\n i am here\n");
	for(int i = 0; i< SIZE; i++)
			schedule_index[i] = 0;
			
	for(int i = 0; i<= SIZE+1; i++)
		available_tam_width[i] = TAM_WIDTH_MAX;
			
	break_point[0] = 0;
	for(int i = 1; i<= SIZE+1; i++)
		break_point [i] = LARGENUMBER;
	// break points [ 0 ,99999999,9999999 ....] intially
	for(int i = 0; i< SIZE; i++)
		session_tsv[i] = layer_tsv [i] = 0;

	int t =0;
	
	long int schedule_start_time = 0;
	while (no_of_core_scheduled < SIZE)
	{
		cout << "schedulerInfo "  << endl;
		for (int i = 0; i < SIZE; ++i)
		{
			cout << scheduler[index][i].corenum <<" " << scheduler[index][i].tam_width << " " << scheduler[index][i].starttime << " " << scheduler[index][i].endtime << endl;
			
		}
		int flag = 1;
		cout << "no_of_core_scheduled " << no_of_core_scheduled << "**********************" << endl;
		while(flag ==1)
		{
			cout << "schedulerInfo "  << endl;
		for (int i = 0; i < SIZE; ++i)
		{
			cout << scheduler[index][i].corenum <<" " << scheduler[index][i].tam_width << " " << scheduler[index][i].starttime << " " << scheduler[index][i].endtime << endl;
			
		}

		cout << "available_tam_width " << endl;
		for (int i = 0; i < SIZE; ++i)
		{
			cout <<  available_tam_width[i] << " " ;
			
		}
		cout << endl;
			cout << "flag is still 1 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
			max_weightage = 0;
			min_weightage = LARGENUMBER;
			flag = 0;
			int jj = 0;
			schedule_start_time = break_point[jj];
			// all break points are shifted in the left
			int current_layer_tsv[SIZE-1];
			if(session)
			{
				for(int i = 0; i< SIZE-1; i++)
					current_layer_tsv [i] = 0;
			}
			for (int kk = 0; kk< SIZE; kk++)
			{
				// check out the current layer tsv in each iteration
				cout << "kk is now " << kk  << "++++++++++++"<< endl;
				cout << "schedule_index" << endl;
				for (int i = 0; i < SIZE; ++i)
				{
					cout << schedule_index[i] << " " ;
				}
				// two loops means which unscheduled cores can be scheduled parallely
					// with the already scheduled core
				// time complexity n*n
				cout << endl;
				// initially all index are 0
				if(schedule_index[kk] == 0 )
				{
					// initialy all tam_widths are TAM_MAX
					if (available_tam_width[jj] >= assigned_tam[kk] )
					{
						int parallel_index[SIZE];
						int finish_index[SIZE];
						for(int aa = 0; aa < SIZE; aa++)
						{
							parallel_index[aa] = 0;
							finish_index[aa] = 0;
						}
						
						for(int zz = 0; zz < SIZE; zz++)
						{
							// Find parallel cores
							if(schedule_index[zz] == 1)
							{
								for(int yy = 0; yy < t; yy++)
								{
									if(scheduler[index][yy].corenum == zz)
									{
										//schedule_start_time = LARGE_NO initially, scheduler [index][yy].starttime = -1
										if(schedule_start_time >= scheduler [index][yy].starttime &&
										 schedule_start_time < scheduler [index][yy].endtime)
											parallel_index[zz] = 1;	

										if(schedule_start_time >= scheduler [index][yy].endtime)
											finish_index[zz] = 1;
										
											
									}
								}
								
							} // if already scheduled
							
							
							

						}

						parallel_index[kk] = 1; 

						
						int tsv_count = tsv_required(parallel_index, assigned_tam, current_layer_tsv);
						
						/*cout << "finish_index" << endl;
						for(int i = 0; i < SIZE; i++)
						{
							printf("%d ", finish_index[i]);
						}
						printf("\n");
						cout << "parallel_index " << endl;
						for(int i = 0; i < SIZE; i++)
						{
							printf("%d ", parallel_index[i]);
						}
						printf("\n");
						cout << "testtime "<<endl;
						for(int i = 0; i < SIZE; i++)
						{
							printf("%ld ", testtime[i]);
						}
						printf("\n");
						cout << " assigned_tam" << endl;
						for(int i = 0; i < SIZE; i++)
						{
							printf("%d ", assigned_tam[i]);
						}
						printf("\n");

						cout << " current_layer_tsv " << endl;

						for(int i = 0; i < SIZE-1; i++)
						{
							printf("%d ", current_layer_tsv[i]);
						}
						printf("\n");
						cout << "session_tsv " << endl;
						for(int i = 0; i < SIZE-1; i++)
						{
							printf("%d ", session_tsv[i]);
						}
						printf("\n");

						cout << "area vector " << endl;
						for(int i = 0; i < SIZE-1; i++)
						{
							printf("%lf ", area[i]);
						}
						cout << endl;*/
						//printf("TSV used : %d\n", tsv_count);

						int cost = area[kk];
						cost = area[kk];				
						int temp_layer_tsv[SIZE-1];
						for(int pp = 0; pp < SIZE-1; pp++)
						{
							temp_layer_tsv[pp] = 0;
						}

						//if(tsv_count <= TSV_MAX)
						//{
						int layer_check = 1;
						int sum = 0;
						// temp_layer_tsv = max(session_tsv,current_layer_tsv)
						for (int pp = 0; pp < SIZE-1; pp++)
						{
							if(current_layer_tsv[pp] < session_tsv[pp])
							{
								//layer_check = 0;
								temp_layer_tsv[pp] = session_tsv[pp];
							}
							else
							{
								temp_layer_tsv[pp] = current_layer_tsv[pp];
							}
							sum = sum + temp_layer_tsv[pp];
						}
						printf("TSV sum is  used : %d\n", sum);
						if(sum <= TSV_MAX || tsv_count == 0)
						{
							//if(session == 1 || tsv_count == 0)
							//	layer_check = 1;
							//if(layer_check)
							
							flag = 1;
							if(cost > max_weightage)
							{							
								
								max_weightage = cost;
								max_index = kk;
								for (int pp = 0; pp < SIZE-1; pp++)
								{
									layer_tsv[pp] =  temp_layer_tsv[pp]; 
								}
								
							}
							
							
						//}
						}

						

						
					}// if there is available tam
				} // if schedule index[kk] is unscheduled
			
				
			}// kk loop ends here
			cout << "schedule_index " << endl;
			for (int i = 0; i < SIZE; ++i)
			{
				cout << schedule_index[i] << " ";
			}
			cout << endl;

			cout << "max_index " << max_index << endl;
			
			if (flag == 0)
			{
				cout <<" flag is 0 break break break )))))))))))))))" << endl;
				break;
			}
			
			
			scheduler [index][t].corenum =  max_index;
			scheduler [index][t].tam_width = assigned_tam[max_index];
			scheduler [index][t].starttime = break_point[jj];
			scheduler [index][t].endtime = break_point[jj] + testtime [max_index];	
			cout << "starttime " << scheduler [index][t].starttime << endl;
			cout << "endtime " << scheduler [index][t].endtime << endl;
			int number_of_brk = 0;
			while(break_point [number_of_brk] != LARGENUMBER)
				number_of_brk++;
			cout << "number_of_brk " << number_of_brk << endl;
			for (int i = 0; i < SIZE+1; ++i)
			{
				cout << break_point[i] << " ";
			}
			cout << endl;
			if(scheduler [index][t].endtime > break_point[number_of_brk - 1])
			{
				if(number_of_brk == 1)
					break_point[number_of_brk] = scheduler [index][t].endtime;
				else
					break_point[number_of_brk-1] = scheduler [index][t].endtime;
			}
			// break_point[number_of_brk] = scheduler [index][t].endtime;
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
				if (break_point[i] >= scheduler[index][t].endtime  )
				{
					insert_index = i;
					//break;
				}	
			}
			cout << "insert index "<< insert_index << endl;
			//inserting breakpoint
			//insert_index = number_of_brk;
			// shifting everything in left
			for (int i = number_of_brk; i>= insert_index; i--)
				available_tam_width[i] = available_tam_width[i-1];	
			// what the fuck happening here ?
			int a = jj;
			while(break_point[a]< scheduler[index][t].endtime )
			{		
				available_tam_width[a] = available_tam_width[a] - assigned_tam[max_index];
				a++;
			}	
			schedule_index[scheduler [index][t].corenum] = 1;
			no_of_core_scheduled++;	
			t++;
			cout << "modified breakpoint s "  << endl;
			for (int i = 0; i < SIZE+1; ++i)
			{
				cout << break_point[i] << " ";
			}
			cout << " t is now " << t << endl;
		} // while flag == 1
		//understand upto this 

		cout << " understand upto this" << "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}" << endl;
		
		
		int latest_schedule_cores[SIZE];
		long int latest_schedule_core_testtime[SIZE];
		int latest_schedule_core_tam[SIZE];	
		int k = 0;
		int no_of_latest_scheduled_core = 0;
		int temp1;
		long int temp2;
		// latest means which cores are scheduled at break point 0
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
		// sort latest schedul cores
		for(int i = 0; i< no_of_latest_scheduled_core-1; i++)
		{
			for(int j = 0 ; j< no_of_latest_scheduled_core-i-1; j++)
			{
				if(latest_schedule_core_testtime[j] <
					latest_schedule_core_testtime[j+1])
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
	
		
		//}	
		// shift all breakpoint left ? why ?
		for (int i =0; i< SIZE+1; i++)
		{
			break_point[i] = break_point[i+1];
			available_tam_width[i] = available_tam_width[i+1];
		}
		session++;
		//printf("\n\n ********************New session - %d breakpoint -  %ld ************************* \n\n", session, break_point[0]);
		for(int i = 0; i < SIZE-1; i++)
		{		
			session_tsv[i] = layer_tsv[i];
			//printf("%d ", session_tsv[i]);
		}
		//printf("\n");
		break_point[SIZE+1] = LARGENUMBER;
		available_tam_width[SIZE+1] = TAM_WIDTH_MAX;	
	} // all cores are scheduled now
	// sort schedular
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

        for (int i = 0; i < SIZE; ++i)
        {
        	/* code */
        	cout << scheduler[index][i].corenum <<" " << scheduler[index][i].tam_width << " " << scheduler[index][i].starttime << " " << scheduler[index][i].endtime << endl;
        }
	

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
	// what is the siniicance of number 64
	// how we are scheduling cores parallely

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
	cout << "returning default , fuck 64 "  << break_point[kk-1] << endl;
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
	
	cout << "returning default , fuck 64 "  << break_point[kk] << endl;
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
	int flag = 1;
	while(flag)
	{
		int tsv_count = 0;
		int temp[SIZE]; 
		for(int jj = 0; jj < SIZE; jj ++)
		{
			temp[jj] = 0;
		}
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
			{
				temp[i] = temp[i] = tam_testtime[i].tam_list[decimal];
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
	/*		for(int i = 0; i < SIZE; i++)
			{
				printf("%d ", temp[i]);
			}*/
			//printf(" swap tsv %d\n", tsv_count);
		}
	}
}

void initialiseparticle(particle *object)
{
	int binary;
	int decimal;
	object->lbesttime = LARGENUMBER;
	
	int flag = 1;
	while(flag)
	{
		int tsv_count = 0;
		int temp[SIZE]; 
		for(int jj = 0; jj < SIZE; jj ++)
		{
			temp[jj] = 0;
		}
		for(int i = 0;i < (SIZE);)
		{
			
			for(int j = (i* BIT_LENGTH); j < ((i* BIT_LENGTH) + BIT_LENGTH); j++)	
				object->info[j]=rand()% 2;
			binary = 0;	
			for(int k = 0; k < BIT_LENGTH ; k++)
				binary = binary + object->info[(i* BIT_LENGTH) + k] * pow (10,BIT_LENGTH - 1 -k);
			decimal	= bin_to_decimal(binary);
			if(decimal< tam_testtime[i]. no_of_tam)
			{
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
			for(int i = 0; i < SIZE; i++)
			{
				//printf("%d ", temp[i]);
			}
			//printf(" init tsv %d\n", tsv_count);
		}
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
   		/*
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
        	}*/
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

	//p1 = fopen ( argv [ 1 ],"w" ) ;
	int NoIteration = atoi ( argv [ 1 ] ) ;
        //TAM_WIDTH_MAX = atoi ( argv [ 3 ] ) ;
        int iteration;

	long int t,mint;
	
	FILE *tamread;
	tamread = fopen("tam_testtime.txt","r");
	for(int ii = 0; ii< SIZE; ii++)
		tam_testtime[ii].no_of_tam =  TAM_INFO[ii];
	cout << "tam_testtime.no_of_tam" << endl;
	for(int ii = 0; ii< SIZE; ii++)
		cout << tam_testtime[ii].no_of_tam << endl;

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
		for(int i=1;i<1;i++)
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
	
		//fprintf(p1,"%ld\n",mint);
                printf("%ld\n",mint);
		
	}
        for ( int i = 0 ; i < SIZE ; i ++ ) {
                printf ("{ %d, %d, %ld, %ld }\n", BestParticle[i].corenum, BestParticle[i].tam_width, BestParticle[i]. starttime, BestParticle[i]. endtime);
        }
	
	

    return 0;
}

