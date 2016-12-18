// C / C++ program for Dijkstra's shortest path algorithm for adjacency
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

extern dijkstra_stats_t stats;

// Driver program to test above functions
int main(int argc,char ** argv)
{

    unsigned int nthreads, source;
    int next_option, test_flag, print_flag;
    char graphfile[256];
    struct Graph* graph;

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
     * Initialize dijkstra stats
     */
     dijkstra_stats_init(&stats);

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

    /*
     * Serial dynamic insertion of nodes
     */

    #ifdef TOTAL  
    //> Init and start wall_timer.
//    wall_timer = timer_init();
//    timer_start(wall_timer);
    #endif

//    dijkstra_uni(graph, source);

    #ifdef TOTAL
    //> Stop wall_timer.
//    timer_stop(wall_timer);
    #endif

    if(print_flag)
	printArr(graph->V);

    /*
     * Print statistics
     */
    fprintf(stdout, "\n\n\n\n");
    fprintf(stdout, "Vertices: \t\t %u\n", graph->V);
    fprintf(stdout, "Edges: \t\t\t %u\n", graph->edges);
    
    double time_elapsed = timer_report_sec(wall_timer);
    fprintf(stdout, "Time elapsed: \t\t %f\n", time_elapsed);
    dijkstra_stats_print(&stats);

    return 0;
}

