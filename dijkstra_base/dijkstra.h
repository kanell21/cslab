#ifndef __DIJKSTRAMT_H_
#define __DIJKSTRAMT_H_

#include <pthread.h>
#include<stdbool.h>

#include "graph.h"
#include "minheap.h"
#include "transaction.h"

// A structure to represent a node in adjacency list
struct node_chars {
    int pos;
    weight_t dist;
    int prev;
    bool visited;
};

extern void dijkstra(struct Graph* graph, int src);
#endif
