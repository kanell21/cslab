#ifndef __BHEAP_H_
#define __BHEAP_H_

#include <stdbool.h>
#include "graph.h"

#define CACHE_LINE_SIZE 64
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
    int ups_decrease_key;
    int decrease_keys;
};
 
extern struct MinHeapNode* newMinHeapNode(int v, int dist);
extern struct MinHeap* createMinHeap(int capacity);
extern void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);
extern void minHeapify(struct MinHeap* minHeap, int idx);
extern int isEmpty(struct MinHeap* minHeap);
extern struct MinHeapNode* extractMin(struct MinHeap* minHeap);
extern void decreaseKey(struct MinHeap* minHeap, int v, int dist);
extern bool isInMinHeap(struct MinHeap *minHeap, int v);
#endif
