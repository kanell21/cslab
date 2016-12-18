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
//extern graph_stats_t stats;
//int count;
struct node_chars *nodes;

/********************* SERIAL ********************/

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

// The main function that calculates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
double total_extract_time = 0;
double total_decrease_time = 0;
double total_insert_time = 0;
double total_update_time = 0;
void dijkstra(struct Graph* graph, int src)
{
//In order to check neighbours' heterogenia
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
    total_insert_time = timer_report_sec(timer);
    #endif
   
   // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    int extracts = 0;
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        #ifdef EXTRACT
        timer_start(timer);
	#endif
		
        struct MinHeapNode minHeapNode = extractMin(minHeap);
		
        #ifdef EXTRACT
     	timer_stop(timer);
        total_extract_time = timer_report_sec(timer);
	#endif
		
        int u = minHeapNode.v; // Store the extracted vertex number
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values

        /*__builtin_prefetch (graph->array[minHeap->array[0]->v].head, 1, 1);
        for(i=0; i< graph->array[minHeap->array[0]->v].neighboors;i++){
        __builtin_prefetch (&nodes[graph->array[minHeap->array[0]->v].head[i].dest], 1, 1); //<<<<<<<<<<<<<<<=========================P R E F E T C H I N G ================>>>>>>>>>>>>>
        }*/
        
        int nb=graph->array[u].neighboors;
		
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
                    nodes[v].prev=u;
					
                    #ifdef DECREASE
             	    timer_start(timer);
    		    #endif
					
                    //begin_transaction(3, &writelock, counter);
                    decreaseKey(minHeap, v, nodes[v].dist);
                    //end_transaction(&writelock);
					
                    #ifdef DECREASE
		   timer_stop(timer);
		   total_decrease_time = timer_report_sec(timer);
    		   #endif
                }
                
            }

        }
    #ifdef UPDATE
    timer_stop(timer);
    total_update_time = timer_report_sec(timer);
    #endif
    }
 
    // print the calculated shortest distances
    printArr(V);
}

void dijkstra_uni(struct Graph* graph, int src)
{
//In order to check neighbours' heterogenia
        

    double time;            //variables for timing
    struct timeval ts,tf;
    int V = graph->V;// Get the number of vertices in graph
    nodes=malloc(V*sizeof(struct node_chars));

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);
    
    // Initialize min heap with all vertices. dist value of all vertices 

    #ifdef INSERT
            gettimeofday(&ts,NULL);
    #endif 

    for (int v = 0; v < V; ++v)
    {
        nodes[v].dist= FLT_MAX;
        //minHeap->array[v] = newMinHeapNode(v, nodes[v].dist);
        //nodes[v].pos = v;
        nodes[v].pos =- 1;
        nodes[v].prev = 0;
        nodes[v].visited = false;
    }
    
    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[0] = newMinHeapNode(src, 0);
    nodes[src].pos = 0;
    nodes[src].dist= 0;
    minHeap->size=1;
    int inserts=0;
    inserts++;
    //printMinheap(minHeap);
    //decreaseKey(minHeap, src, nodes[src].dist);
    
    // Initially size of min heap is equal to V
    
    #ifdef INSERT
            gettimeofday(&tf,NULL);
            time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
            total_insert_time=time;
    #endif
    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    int extracts=0;

    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        /*double time;            //variables for timing
        struct timeval ts,tf;
                */
        #ifdef EXTRACT
            gettimeofday(&ts,NULL);
        #endif
        struct MinHeapNode minHeapNode = extractMin(minHeap);
        
    	extracts++;
        #ifdef EXTRACT
        gettimeofday(&tf,NULL);
                time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
                total_extract_time+=time;
        #endif
        int u = minHeapNode.v; 
        nodes[u].visited=true;

        // Store the extracted vertex number
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        //__builtin_prefetch (graph->array[minHeap->array[0]->v].head, 1, 1);
        int nb=graph->array[u].neighboors;
        printf("Extracted vertex %d with value=%d has %d neighboors \n",u,nodes[u].dist,nb);
        
        #ifdef UPDATE
        gettimeofday(&ts,NULL);
        #endif  
        
        
            for(int i=0;i<nb;i++)
            {
                int v = graph->array[u].head[i].dest ;
                //printf("Neigbour %d of %d \n",v,u);
                if(nodes[v].visited==false){
                    if(!isInMinHeap(minHeap,v)){
                     inserts++;
                     printf("Inserted Vertex %d \n",v);   
                     MinHeap_insert(minHeap,newMinHeapNode(v,nodes[u].dist+graph->array[u].head[i].weight));
                     nodes[v].dist=nodes[u].dist+graph->array[u].head[i].weight;
                    }
                    // If shortest distance to v is not finalized yet, and distance to v
                    // through u is less than its previously calculated distance
                    else if (graph->array[u].head[i].weight+ nodes[u].dist < nodes[v].dist)
                    {
                        nodes[v].dist = nodes[u].dist + graph->array[u].head[i].weight;
                        //printf("About to Decrease key of Vertex %d to value %d \n",v,nodes[v].dist);

                        nodes[v].prev=u;
                        // update distance value in min heap also
                        #ifdef DECREASE
                           gettimeofday(&ts,NULL);  
                        #endif
                        decreaseKey(minHeap, v, nodes[v].dist);
                        #ifdef DECREASE
                        gettimeofday(&tf,NULL);
                        time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
                        total_decrease_time+=time;
                        #endif
                    }
                
                }
            }
        
     #ifdef UPDATE
     gettimeofday(&tf,NULL);
     time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
     total_update_time+=time;
     #endif
    }
    printArr(V);
    printf("ExtractMins=%d Inserts=%d",extracts,inserts);

} 
