#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include<string.h>

#include "graph.h"
#include "minheap.h"

// Read graph
struct Graph* graph_read(const char *filename)
{
    FILE *fp;
    fp = fopen(filename,"r");
    char line[50];
    char *token;
    int V;
    int dest;
    int source;
    weight_t weight;
    int counter = 1;
    struct Graph* graph = NULL;
    int edges = 0;
	
    while ( fgets ( line, sizeof line, fp ) != NULL ) /* read a line */
      {
        token=strtok(line," ");

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
            weight=atof(token);
            //printf("Weight=%d\n",weight);
            edges++;
	    addEdge(graph, source, dest, weight);
          
        }
      counter++;
      }   
	  
    return graph;
}

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, weight_t weight)
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
    graph->edges = 0;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i){      
      graph->array[i].head = NULL;
      graph->array[i].neighboors = 0;
    }
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, weight_t weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    if(graph->array[src].head==NULL) 
		graph->array[src].head = (struct AdjListNode *) malloc(sizeof(struct AdjListNode));
    else 
		graph->array[src].head = (struct AdjListNode *) realloc(graph->array[src].head, (graph->array[src].neighboors+1) * sizeof(struct AdjListNode));
    
    graph->array[src].head[graph->array[src].neighboors] = *newNode;
    graph->array[src].neighboors++;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    
    if(graph->array[dest].head==NULL) 
		graph->array[dest].head = (struct AdjListNode *) malloc(sizeof(struct AdjListNode));
    else 
		graph->array[dest].head = (struct AdjListNode *) realloc(graph->array[dest].head, (graph->array[dest].neighboors+1) * sizeof(struct AdjListNode));
    
    graph->array[dest].head[graph->array[dest].neighboors] = *newNode;
    graph->array[dest].neighboors++;

    graph->edges++;
}
