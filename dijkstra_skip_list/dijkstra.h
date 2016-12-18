#ifndef __DIJKSTRAMT_H_
#define __DIJKSTRAMT_H_

#include <pthread.h>
#include<stdbool.h>

#include "graph.h"
#include "sl_pq.h"
#include "transaction.h"

// A structure to represent a node in adjacency list
struct node_chars {
    int pos;
    weight_t dist;
    int prev;
    bool visited;
};

typedef struct dijkstra_stats_st{
    double total_extract_time;
    double total_decrease_time;
    double total_insert_time;
    double total_update_time;
    int extracts;
    int inserts;
    int decrease_keys;
    int ups_decrease_key;
} dijkstra_stats_t;

extern void printArr(int V);
extern void dijkstra_stats_init(dijkstra_stats_t *stats);
extern void djikstra_stats_print(dijkstra_stats_t *stats);
extern void dijkstra(struct Graph* graph, int src);
#endif
