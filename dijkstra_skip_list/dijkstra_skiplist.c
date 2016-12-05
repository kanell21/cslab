#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 
#include <sys/time.h>
#include <errno.h>
#include "sl_pq.h"
#include "util.h"
#include <float.h>


#define OK      0
#define NOTOK   -1


int decrease_keys=0;


//<============================================================================================================================================>
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    int i;
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

void printArr(int dist[], int n)
{
    int i;
    printf("Vertex   Distance from Source\n");
    for (i = 0; i < n; ++i){
        printf("%d \t\t %d\n", i, dist[i]);}
}

double total_extract_time=0;
double total_decrease_time=0;
double total_insert_time=0;
double total_update_time=0;
void dijkstra(struct Graph* graph, int src)
{
 // printf("Hereeeeeeeeeeeeeeeeeeeeeeeeeee");
    int V = graph->V;// Get the number of vertices in graph
    int *dist;  
    int check;    // dist values used to pick minimum weight edge in cut
    double time;            //variables for timing
    struct timeval ts,tf;

    dist = malloc(V * sizeof(int));
    if (!dist) {
        perror("Malloc failed");
        exit(1);
    }

    // minHeap represents set E
    //struct MinHeap* minHeap = createMinHeap(V);
      pq_t *pq = pq_create(V);
      int v; 
    // Initialize min heap with all vertices. dist value of all vertices 
	#ifdef INSERT
    		gettimeofday(&ts,NULL);
	#endif  
    for (v = 0; v < V; ++v)
    {
        if(v==src)
        { dist[src]=0; 
          pq_insert(pq,0.0,v);
          //print_list(fheap->Min);
          }
        else{
        dist[v] = INFINITY;
        //minHeap->array[v] = newMinHeapNode(v, dist[v]);
        //minHeap->pos[v] = v;
        //print_list(fheap->Min);
        pq_insert(pq,FLT_MAX,v);
        //printf("Vertex:%d inserted",v);
        }
       // printf("Node[%d]=%d \n",v,fheap->nodes[v]);
    }
	#ifdef INSERT
        	gettimeofday(&tf,NULL);
        	time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
        	total_insert_time=time;
	#endif
     // fheap->Min->Left->Right = NULL;
      //fheap->Min->Left = NULL;
      //print_list(fheap->Min);
     // printf("insert done\n") ;
    // Make dist value of src vertex as 0 so that it is extracted first
   
 
    // Initially size of min heap is equal to V
 
    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    int extracts=0;
    while (pq->curr_size > 0)
    {
    
        // Extract the vertex with minimum distance value
   //     printf("About to Extract");
        //double time;            //variables for timing
        //struct timeval ts,tf;
	#ifdef EXTRACT
        	gettimeofday(&ts,NULL);
	#endif
        int u =pq_delete_min(pq);
	#ifdef EXTRACT
     	    gettimeofday(&tf,NULL);
        	time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
        	total_extract_time+=time;
	#endif
	 // Store the extracted vertex number
        struct AdjListNode* pCrawl = graph->array[u].head;
	#ifdef UPDATE
        	gettimeofday(&ts,NULL);
	#endif
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
 
            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if ( pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
 
             
			   // update distance value in min heap also
		
                decrease_keys++;

		#ifdef DECREASE
         	      gettimeofday(&ts,NULL);	
		#endif
	
                pq_decrease_key(pq,v,dist[v]);
	
		#ifdef DECREASE
 	              gettimeofday(&tf,NULL);
        	      time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
                total_decrease_time+=time;
		#endif
                //printf("Decreased node:%d to dist=%d\n",v,dist[v]);
            }
            pCrawl = pCrawl->next;
        }
	#ifdef UPDATE
        gettimeofday(&tf,NULL);
        time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
        total_update_time+=time;
	#endif
    }
 
    // print the calculated shortest distances
    //printArr(dist, V);
}




int main(int argc,char *argv[])
{
    // create the graph given in above fugure
  //printf("Start\n");
    FILE *fp;
    fp=fopen(argv[1],"r");
    char line[50];
    char *token;
    int V;
    int dest;
    int source;
    int weight;
    int counter=1;
    long int edges=0;
    struct Graph* graph;
    double time;            //variables for timing
    struct timeval ts,tf;
     //printf("Degrees=%d",degrees(100000000));
          //printf("Start2\n");
          while ( fgets ( line, sizeof(line), fp ) != NULL ) /* read a line */
      {
        //printf("Start3\n");
        token=strtok(line," ");
        if(token==NULL) break;
       //printf("Token=%s\n",token);
          //printf("Start4\n");

        if(strcmp(token,"c")==0 && counter==2)
        {

           token=strtok(NULL," ");
           //printf("Token=%s\n",token);
           token=strtok(NULL," ");
          // printf("Token=%s\n",token);
           token=strtok(NULL," ");
           //printf("Token=%s\n",token);
           token=strtok(NULL," ");
          // printf("Token=%s\n",token);
           V=atoi(token);
           printf("V=%d\n",V);
           //printf("V=%d",V);
           graph = createGraph(V);
        }
        else if(strcmp(token,"a")==0)
        { 

            //printf("Here");
            token=strtok(NULL," ");
            source=atoi(token);
            //printf("Source=%d\t",source);



            token=strtok(NULL," ");
            dest=atoi(token);
            //printf("Dest=%d\n",dest);



            token=strtok(NULL," ");
            weight=atoi(token);
            //printf("Weight=%d\n",weight);
          addEdge(graph, source,dest,weight);
          edges++;
          
        }


      counter++;



      }
      fclose(fp);       
   // printf("Edges:%d",edges);
    //printf("Degrees=%d",degrees(100000000));
    #ifdef TOTAL
    	gettimeofday(&ts,NULL);
    #endif 	
    //printf("Dijkstra");
    dijkstra(graph,1);
    #ifdef TOTAL	
   	 gettimeofday(&tf,NULL);
    time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
    #endif
    #ifdef TOTAL
    printf("Time=%lf:",time);
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
    printf("Vertices:%d:Edges:%d\n",V,edges);
    printf("Decrease keys =%d ",decrease_keys);
    //printf("Time to decrease=%lf \n",total_decrease_time);
    //printf("Update time: %lf",total_update_time);

    return 0;
}
