#ifndef __GRAPH_H_
#define __GRAPH_H_

typedef float weight_t;

struct AdjListNode
{
    int dest;
    weight_t weight;
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
    unsigned int V;
    struct AdjList* array;
};

extern struct Graph* graph_read(const char *filename);
extern struct AdjListNode* newAdjListNode(int dest, weight_t weight);
extern struct Graph* createGraph(int V);
extern void addEdge(struct Graph* graph, int src, int dest, weight_t weight);
#endif
