/ C / C++ program for Dijkstra's shortest path algorithm for adjacency
// list representation of graph
 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h> 
#include <sys/time.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include "dijkstra.h"
#include "graph.h"
#include"timers_lib.h"

// Driver program to test above functions
int main(int argc,char ** argv)
{

	unsigned int nthreads, source;
	int next_option, test_flag, print_flag;
	char graphfile[256];
	struct Graph* graph;
    double time;            //variables for timing
    struct timeval ts,tf;

	if(argc==1){
		printf("Usage: ./dijkstra --graph <graphfile>\n"
				"\t\t --nthreads <nthreads>\n" 
				"\t\t --source <source>\n");
		exit(EXIT_FAILURE);
	}
	source = 0;
	nthreads = 1;

	print_flag = 0;
	test_flag = 0;
	source = 0;
	nthreads = 1;

	/* getopt stuff */
	const char* short_options = "g:n:s:pt";
	const struct option long_options[]={
		{"graph", 1, NULL, 'g'},
		{"nthreads", 1, NULL, 'n'},
		{"source", 1, NULL, 's'},
		{"print", 0, NULL, 'p'},
		{"test", 0, NULL, 't'},
		{NULL, 0, NULL, 0}
	};

	do {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);

		switch(next_option){
			case 'n':
				nthreads = atoi(optarg);
				break;

			case 't':
				test_flag = 1;
				break;

			case 'p':
				print_flag = 1;
				break;

			case 's':
				source = atoi(optarg);
				break;

			case 'g':
				sprintf(graphfile, "%s", optarg);
				break;
			
			case '?':
				fprintf(stderr, "Unknown option!\n");
				exit(EXIT_FAILURE);

			case -1:	//Done with options
				break;	

			default:	//Unexpected error
				exit(EXIT_FAILURE);
		}

	}while(next_option != -1);

	graph = graph_read(graphfile);
	fprintf(stdout, "Read graph\n\n");


	if(source > graph->V - 1){
		printf("Source is invalid !\n");
		exit(EXIT_FAILURE);
	}

    /*
     * Serial
     */

    #ifdef TOTAL  
    //> Init and start wall_timer.
    timer_tt *wall_timer = timer_init();
    timer_start(wall_timer);
    #endif

    dijkstra(graph, source);

    #ifdef TOTAL
    //> Stop wall_timer.
    timer_stop(wall_timer);
    #endif

    #ifdef TOTAL
    //> Print elapsed time.
    double time_elapsed = timer_report_sec(wall_timer);
    printf("Time elapsed: %lf\n", time_elapsed);
    #endif
    
    #ifdef DECREASE
    printf("total_decrease_time=%lf:",total_decrease_time);
    #endif
    #ifdef UPDATE
    printf("total_update_time=%lf:",total_update_time);
    #endif
    #ifdef INSERT
    printf("total_insert_time=%lf:",total_insert_time);
    #endif
    #ifdef EXTRACT
    printf("total_extract_time=%lf:",total_extract_time);
    #endif

    //printf("Vertices:%d:Edges:%d\n", graph->V, edges);
    //printf("Decrease_keys=%d ,Ups_decrease_key=%d \n",decrease_keys,ups_decrease_key);
    //printf("Time to decrease=%lf \n",total_decrease_time);
   // printf("Update time: %lf",total_update_time);
    return 0;

}

