#include <stdlib.h>
#include <stdio.h>
#include "minheap.h"
#include "dijkstra.h"

/*
 * Global vars
 */
extern struct node_chars *nodes;
extern dijkstra_stats_t stats;

// A utility function to create a new Min Heap Node
struct MinHeapNode newMinHeapNode(int v, weight_t dist)
{
    struct MinHeapNode minHeapNode;

    minHeapNode.v = v;
    minHeapNode.dist = dist;
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
         (struct MinHeapNode*) malloc(capacity * sizeof(struct MinHeapNode));
   
    return minHeap;
}
 
// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode* a, struct MinHeapNode* b)
{
    struct MinHeapNode t;
    t.v = a->v;
    t.dist = a->dist;

    a->v = b->v;
    a->dist = b->dist;

    b->v = t.v;
    b->dist = t.dist;
   
    return;
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
    struct MinHeapNode smallestNode;
    struct MinHeapNode idxNode ;
	
    if (left < minHeap->size &&
        minHeap->array[left].dist < minHeap->array[smallest].dist )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right].dist < minHeap->array[smallest].dist )
      smallest = right;
 
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap        
        smallestNode = minHeap->array[smallest];
        idxNode = minHeap->array[idx];
 
        // Swap positions
        nodes[smallestNode.v].pos = idx;
        nodes[idxNode.v].pos = smallest;
 
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
struct MinHeapNode extractMin(struct MinHeap* minHeap)
{
//    if (isEmpty(minHeap))
//        return NULL;
 
    // Store the root node
    struct MinHeapNode root = minHeap->array[0];
 
    // Replace root node with last node
    struct MinHeapNode lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    // Update position of last node
    nodes[root.v].pos = minHeap->size-1;
    nodes[lastNode.v].pos = 0;
 
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return root;
}

// Function to decrease dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, weight_t dist)
{
    // Get the index of v in  heap array
    int i = nodes[v].pos;
    stats.decrease_keys++;
	
    // Get the node and update its dist value
    minHeap->array[i].dist = dist;
 
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i].dist < minHeap->array[(i - 1) / 2].dist)
    {
        stats.ups_decrease_key++;
        // Swap this node with its parent
        nodes[minHeap->array[i].v].pos= (i-1)/2;
        nodes[minHeap->array[(i-1)/2].v].pos = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
 
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

void MinHeap_insert(struct MinHeap* minHeap, struct MinHeapNode inode)
{
    
    int i, j;
    struct MinHeapNode ParentNode;
   // printf("About to Insert Vertex %d with dist= %d \n",inode->v,inode->dist);
    
    i = (minHeap->size)++;
   // printf("Vertex %d Initial position= %d \n",inode->v,i);

    
    while(i >= 1) {
        
        j = i / 2;
        ParentNode = minHeap->array[j];
        
        if(inode.dist  >= ParentNode.dist) break;

        minHeap->array[i] = ParentNode;
        nodes[ParentNode.v].pos = i;
        i = j;
    }
    minHeap->array[i] = inode;
    nodes[inode.v].pos = i;
}  
