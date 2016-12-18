#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <sched.h>
#include <stdbool.h>
#include <float.h>

#include "graph.h"
#include "timers_lib.h"
#include "sl_pq.h"
#include "dijkstra.h"

/* Global vars */
struct node_chars *nodes;
dijkstra_stats_t stats;
/********************na*ERIAL ********************/

static void setaffinity_oncpu(int cpu)
{
    cpu_set_t cpu_mask;
    int err;

    CPU_ZERO(&cpu_mask);
    CPU_SET(cpu, &cpu_mask);

    err = sched_setaffinity(0, sizeof(cpu_set_t), &cpu_mask);
    if (err) {
        perror("sched_setaffinity");
        exit(1);
    }
}

// A utility function used to print the solution
void printArr(int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %lf\n", i, nodes[i].dist);
}

// Initialize global statistics
void dijkstra_stats_init(dijkstra_stats_t *stats)
{
    stats->total_extract_time = 0.0;
    stats->total_decrease_time = 0.0;
    stats->total_insert_time = 0.0;
    stats->total_update_time = 0.0;
    stats->extracts = 0;
    stats->inserts = 0;
    stats->decrease_keys = 0;
    stats->ups_decrease_key = 0;
}

void dijkstra_stats_print(dijkstra_stats_t *stats)
{
	fprintf(stdout, "Extract time: \t\t %f\n", stats->total_extract_time);
	fprintf(stdout, "Decrease time: \t\t %f\n", stats->total_decrease_time);
	fprintf(stdout, "Insert time: \t\t %f\n", stats->total_insert_time);
	fprintf(stdout, "Update time: \t\t %f\n", stats->total_update_time);
	fprintf(stdout, "Extracts: \t\t %u\n", stats->extracts);
	fprintf(stdout, "Inserts: \t\t %u\n", stats->inserts);
	fprintf(stdout, "Decrease keys: \t\t %u\n", stats->decrease_keys);
	fprintf(stdout, "Ups decrease keys: \t %u\n", stats->ups_decrease_key);
	fprintf(stdout, "\n\n\n");
}

// The main function that calculates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph* graph, int src)
{
    timer_tt *timer = timer_init();
    int V = graph->V;// Get the number of vertices in graph
    nodes = malloc(V * sizeof(struct node_chars));

    pq_t *pq = pq_create(V);
    
    // Initialize priority queue with all vertices. dist value of all vertices 

    #ifdef INSERT
    timer_start(timer);
    #endif 

    for (int v = 0; v < V; ++v)
    {
        nodes[v].pos = v;
        nodes[v].prev = 0;
	if(v == src){ 
	  nodes[v].dist= 0.0;
          pq_insert(pq, 0.0, v);
        }else{
          nodes[v].dist= FLT_MAX;
          pq_insert(pq, FLT_MAX, v);
        }
    }
    
    // Initially size of min heap is equal to V

    #ifdef INSERT
    timer_stop(timer);
    stats.total_insert_time = timer_report_sec(timer);
    #endif
   
   // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (pq->curr_size > 0)
    {
        // Extract the vertex with minimum distance value
        #ifdef EXTRACT
        timer_start(timer);
	#endif
		
        int u = pq_delete_min(pq);
		
        #ifdef EXTRACT
     	timer_stop(timer);
        stats.total_extract_time = timer_report_sec(timer);
	#endif
	
	stats.extracts++;
	
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values

        int nb = graph->array[u].neighboors;
		
        #ifdef UPDATE
        timer_start(timer);
	#endif	
        
	if(nodes[u].dist != FLT_MAX){
            //#pragma omp parallel for private(i) 
            for(int i=0;i<nb;i++)
            {
                int v = graph->array[u].head[i].dest ;
     
                // If shortest distance to v is not finalized yet, and distance to v
                // through u is less than its previously calculated distance
                if (pq->where_in_pq[v] != NULL && 
                                               graph->array[u].head[i].weight+ nodes[u].dist < nodes[v].dist)
                {
                    // update distance value in min heap also
		    nodes[v].dist = nodes[u].dist + graph->array[u].head[i].weight;
                    nodes[v].prev = u;
					
                    #ifdef DECREASE
             	    timer_start(timer);
    		    #endif
					
                    pq_decrease_key(pq, v, nodes[v].dist);
					
                    #ifdef DECREASE
		    timer_stop(timer);
		    stats.total_decrease_time = timer_report_sec(timer);
    		    #endif
                }
                
            }

        }
    
    #ifdef UPDATE
    timer_stop(timer);
    stats.total_update_time = timer_report_sec(timer);
    #endif    
   }
}
 
