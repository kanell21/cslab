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


int main(){
    struct Graph* graph;
    graph=createGraph(5);
    addEdge(graph,0,1,2);
    addEdge(graph,0,2,2);
    addEdge(graph,0,3,4);
    addEdge(graph,0,4,3);
    printf("Neigbours of 0 are %d\n",graph->array[0].neighboors);
    printf("Neigbour 0 of 0 are %d\n",graph->array[0].head);
    printf("Neigbour 1 of 0 are %d\n",graph->array[0].head+1);
    printf("Neigbour 2 of 0 are %d\n",graph->array[0].head+2);
    printf("Neigbour 3 of 0 are %d\n",graph->array[0].head+3);
 















}



