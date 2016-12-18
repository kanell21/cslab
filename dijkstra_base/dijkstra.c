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
#include "minheap.h"
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

    //omp_set_num_threads(2);
    //omp_init_lock(&writelock);
    //count_abort_t *counter;
    // minHeap represents set E

    struct MinHeap* minHeap = createMinHeap(V);
    
    // Initialize min heap with all vertices. dist value of all vertices 

    #ifdef INSERT
    timer_start(timer);
    #endif 

    for (int v = 0; v < V; ++v)
    {
        nodes[v].dist= FLT_MAX;
        minHeap->array[v] = newMinHeapNode(v, nodes[v].dist);
        nodes[v].pos = v;
        nodes[v].prev=0;
    }
    
    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, nodes[src].dist);
    nodes[src].pos = src;
    nodes[src].dist= 0;
    decreaseKey(minHeap, src, nodes[src].dist);
    
    // Initially size of min heap is equal to V
    minHeap->size = V;

    #ifdef INSERT
    timer_stop(timer);
    stats.total_insert_time = timer_report_sec(timer);
    #endif
   
   // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        #ifdef EXTRACT
        timer_start(timer);
	#endif
		
        struct MinHeapNode minHeapNode = extractMin(minHeap);
		
        #ifdef EXTRACT
     	timer_stop(timer);
        stats.total_extract_time = timer_report_sec(timer);
	#endif
	
	stats.extracts++;
	
        int u = minHeapNode.v; // Store the extracted vertex number
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
                if (isInMinHeap(minHeap, v) && 
                                               graph->array[u].head[i].weight+ nodes[u].dist < nodes[v].dist)
                {
                    // update distance value in min heap also
		    nodes[v].dist = nodes[u].dist + graph->array[u].head[i].weight;
                    nodes[v].prev = u;
					
                    #ifdef DECREASE
             	    timer_start(timer);
    		    #endif
					
                    //begin_transaction(3, &writelock, counter);
                    decreaseKey(minHeap, v, nodes[v].dist);
                    //end_transaction(&writelock);
					
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

void dijkstra_uni(struct Graph* graph, int src)
{
    timer_tt *timer;
    int V = graph->V;// Get the number of vertices in graph
    nodes = malloc(V*sizeof(struct node_chars));

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
    
    // Initialize min heap with all vertices. dist value of all vertices 

    #ifdef INSERT
    timer_start(timer);
    #endif 

    for (int v = 0; v < V; ++v)
    {
        nodes[v].dist= FLT_MAX;
        nodes[v].pos =- 1;
        nodes[v].prev = 0;
        nodes[v].visited = false;
    }
    
    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[0] = newMinHeapNode(src, 0);
    nodes[src].pos = 0;
    nodes[src].dist= 0.0;
    minHeap->size=1;
    
    stats.inserts++;
    // Initially size of min heap is equal to V
    #ifdef INSERT
    timer_stop(timer);            
    stats.total_insert_time = timer_report_sec(timer);
    #endif
    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.

    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        /*double time;            //variables for timing
        struct timeval ts,tf;
                */
        #ifdef EXTRACT
        timer_start(timer);
 	#endif
        struct MinHeapNode minHeapNode = extractMin(minHeap);
        
    	stats.extracts++;

        #ifdef EXTRACT
        timer_stop(timer);
	stats.total_extract_time = timer_report_sec(timer);
        #endif

        int u = minHeapNode.v; 
        nodes[u].visited=true;

        // Store the extracted vertex number
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        int nb = graph->array[u].neighboors;
        
        #ifdef UPDATE
        timer_start(timer);
        #endif  
        
        
            for(int i=0;i<nb;i++)
            {
                int v = graph->array[u].head[i].dest ;
                
		if(nodes[v].visited == false){
                    if(!isInMinHeap(minHeap,v)){
                        stats.inserts++;
                     
			MinHeap_insert(minHeap,newMinHeapNode(v,nodes[u].dist+graph->array[u].head[i].weight));
                        nodes[v].dist = nodes[u].dist + graph->array[u].head[i].weight;
                    }
                    // If shortest distance to v is not finalized yet, and distance to v
                    // through u is less than its previously calculated distance
                    else if (graph->array[u].head[i].weight+ nodes[u].dist < nodes[v].dist)
                    {
                        nodes[v].dist = nodes[u].dist + graph->array[u].head[i].weight;

                        nodes[v].prev=u;
                        // update distance value in min heap also
                        #ifdef DECREASE
                        timer_start(timer);  
                        #endif
                        decreaseKey(minHeap, v, nodes[v].dist);
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
    //printArr(V);

} 
