// C / C++ program for Dijkstra's shortest path algorithm for adjacency
// list representation of graph
 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h> 
#include <sys/time.h>
#include <stdbool.h>
// A structure to represent a node in adjacency list
struct node_chars {
    int pos;
    int dist;
    int prev;

}*nodes;   ///array of structs for cache efficiency


struct AdjListNode
{
    int dest;
    int weight;
};
 
// A structure to represent an adjacency liat
struct AdjList
{
    int neighboors;
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
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i){      
      graph->array[i].head = NULL;
        graph->array[i].neighboors=0;}
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    if(graph->array[src].head==NULL) graph->array[src].head=(struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    else graph->array[src].head=(struct AdjListNode *)realloc(graph->array[src].head,(graph->array[src].neighboors+1)*sizeof(struct AdjListNode));
    graph->array[src].head[graph->array[src].neighboors] = *newNode;
    graph->array[src].neighboors++;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
     if(graph->array[dest].head==NULL) graph->array[dest].head=(struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    else graph->array[dest].head=(struct AdjListNode *)realloc(graph->array[dest].head,(graph->array[dest].neighboors+1)*sizeof(struct AdjListNode));
    graph->array[dest].head[graph->array[dest].neighboors] = *newNode;
    graph->array[dest].neighboors++;
    
}
 
// Structure to represent a min heap node
struct MinHeapNode
{
    int  v;
    int dist;
};
 
// Structure to represent a min heap
struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap    // This is needed for decreaseKey()
    struct MinHeapNode **array;
};
 
// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}
 
// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
 
// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    struct MinHeapNode *smallestNode;
    struct MinHeapNode *idxNode ;
    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;
 
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap        
        smallestNode=minHeap->array[smallest];
        idxNode= minHeap->array[idx];
 
        // Swap positions
        nodes[smallestNode->v].pos = idx;
        nodes[idxNode->v].pos = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}
 
// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
 
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
 
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    // Update position of last node
    nodes[root->v].pos = minHeap->size-1;
    nodes[lastNode->v].pos = 0;
 
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return root;
}
 int ups_decrease_key=0;
 int decrease_keys=0;
// Function to decreasy dist vaaue of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // Get the index of v in  heap array
    int i = nodes[v].pos;
     decrease_keys++;
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;
 
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        ups_decrease_key++;
        // Swap this node with its parent
        nodes[minHeap->array[i]->v].pos= (i-1)/2;
        nodes[minHeap->array[(i-1)/2]->v].pos = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
        // move to parent index
        i = (i - 1) / 2;
    }
}
 
// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (nodes[v].pos < minHeap->size)
     return true;
   return false;
}
 
// A utility function used to print the solution
void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}
 
// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
double total_extract_time=0;
double total_decrease_time=0;
double total_insert_time=0;
double total_update_time=0;
void dijkstra(struct Graph* graph, int src)
{
//In order to check neighbours' heterogenia
	    
    int i;
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
        nodes[v].dist= INT_MAX;
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
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        #ifdef EXTRACT
     	        gettimeofday(&tf,NULL);
        		time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
        		total_extract_time+=time;
	    #endif
        int u = minHeapNode->v; // Store the extracted vertex number
        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values

    //<====================================================P R E F E T C H  0  S P E C U L A T I V E L Y ======================================================================>

        __builtin_prefetch (graph->array[minHeap->array[0]->v].head, 1, 1);
        __builtin_prefetch (&nodes[u], 1, 1);
        for(i=0; i< graph->array[minHeap->array[0]->v].neighboors;i++){
        __builtin_prefetch (&nodes[graph->array[minHeap->array[0]->v].head[i].dest], 1, 1);
        }


    //<===============================================================================================>




    //<<<<<<<<<<<<<<=======================================P R E F E T C H     R A N D O M L Y     B E T W E E N   0   A N D   1     =================================>>>>>>>>>>


        /*
        int random=rand()%2;
        
        if (random==1){
                for(i=0; i< graph->array[minHeap->array[0]->v].neighboors;i++){
                     __builtin_prefetch (&nodes[graph->array[minHeap->array[0]->v].head[i].dest], 1, 1);
                }   
            }
        else {

                for(i=0; i< graph->array[minHeap->array[1]->v].neighboors;i++){
                     __builtin_prefetch (&nodes[graph->array[minHeap->array[1]->v].head[i].dest], 1, 1);
                }
            }
            */
        //==========================================================================================================================================================================>



        int nb=graph->array[u].neighboors;
        #ifdef UPDATE
        	gettimeofday(&ts,NULL);
		#endif	
        
        if(nodes[u].dist != INT_MAX){
            for(int i=0;i<nb;i++)
            {
                int v = graph->array[u].head[i].dest ;
     
                // If shortest distance to v is not finalized yet, and distance to v
                // through u is less than its previously calculated distance
                if (isInMinHeap(minHeap, v) && 
                                               graph->array[u].head[i].weight+ nodes[u].dist < nodes[v].dist)
                {
                    nodes[v].dist = nodes[u].dist + graph->array[u].head[i].weight;
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
 
    // print the calculated shortest distances
   // printArr(dist, V);
}
 
 
// Driver program to test above functions
int main(int argc,char ** argv)
{
    // create the graph given in above fugure
    FILE *fp;
    fp=fopen(argv[1],"r");
    char line[50];
    char *token;
    int V;
    int dest;
    int source;
    int weight;
    int counter=1;
    struct Graph* graph;
    double time;            //variables for timing
    struct timeval ts,tf;
    int edges=0;
          while ( fgets ( line, sizeof line, fp ) != NULL ) /* read a line */
      {
        token=strtok(line," ");
        //printf("Token=%s\n",token);


        if(strcmp(token,"c")==0 && counter==2)
        {

           token=strtok(NULL," ");
          // printf("Token=%s\n",token);
           token=strtok(NULL," ");
          // printf("Token=%s\n",token);
           token=strtok(NULL," ");
           //printf("Token=%s\n",token);
           token=strtok(NULL," ");
          // printf("Token=%s\n",token);
           V=atoi(token);
          // printf("V=%d",V);
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
            edges++;
          addEdge(graph, source,dest,weight);
          
        }


      counter++;



      }       


    #ifdef TOTAL  
    gettimeofday(&ts,NULL);
    #endif
    dijkstra(graph, 1);
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
    printf("Decrease_keys=%d ,Ups_decrease_key=%d \n",decrease_keys,ups_decrease_key);
    //printf("Time to decrease=%lf \n",total_decrease_time);
   // printf("Update time: %lf",total_update_time);
    return 0;
}
