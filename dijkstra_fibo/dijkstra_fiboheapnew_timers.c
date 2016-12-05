#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 
#include <sys/time.h>
#include <errno.h>

#define OK      0
#define NOTOK   -1

 struct FiboNode{
      
       struct FiboNode *Left;
       struct FiboNode *Right;
       struct FiboNode *Parent;
       struct FiboNode *Child;
       int rank;
       bool marked;
       int v;
       long distance;
 };


 struct FiboHeap{


      struct FiboNode *Min;
      struct FiboNode *LeftMost;
      struct FiboNode **trees;
      struct FiboNode **nodes;
      long int max_nodes,max_trees;
      int current_nodes,current_trees;
      int marked_nodes;
      
 };


typedef struct FiboNode FiNd;
typedef struct FiboHeap FiHp;

int  degrees(int n){


  double gr = 1.618033988749895;
  double result=log2(n)/log2(gr);
  return (int)result;

}

FiNd* newFiboNode(int dest,int value){
      FiNd *node;
      node=(FiNd*)malloc(sizeof(FiNd));
      node->v=dest;
      node->distance=value;
      node->Left=NULL;
      node->Right=NULL;
      node->Parent=NULL;
      node->Child=NULL;
      node->marked=false;
      node->rank=0;
      return node;
}

FiHp* newFiboHeap(long int V){
//   printf("here");
   FiHp* heap;
   heap=(FiHp*)malloc(sizeof(FiHp));
//   if(heap!=NULL) perror("Malloc failed");
   heap->Min=NULL;
   heap->max_trees = degrees(V);
   heap->max_nodes = V;
   heap->trees =(FiNd**) malloc(heap->max_trees*sizeof(FiNd*));
   heap->nodes =(FiNd**)malloc(V*sizeof(FiNd*));
   if(heap->nodes==NULL) perror("Malloc failed");
   heap->current_nodes=0;
   heap->current_trees=0;
   return heap;
}


//<============================= Insert Element to the Left of the root list and update Min and LeftMost ====================================>



void Insert(FiNd *NewNode,FiHp *FiboHeap)
 {
 	  
      if (NewNode == NULL) return;
 
// If the heap is currently empty, then new node becomes singleton
 // circular root list
 
    if (FiboHeap->Min == NULL){
    FiboHeap->Min= NewNode;
    NewNode->Left= NewNode;
    NewNode->Right= NewNode;
      }
      else
      {
 // Pointers from NewNode set to insert between fiboheap->Minand MinRoot->Right

        NewNode->Right = FiboHeap->Min->Right;
        NewNode->Left = FiboHeap->Min;
 
 // Set Pointers to NewNode  
      NewNode->Left->Right = NewNode;
         NewNode->Right->Left = NewNode;
 
 // The new node becomes new fiboheap->Minif it is less than current MinRoot
 
         if (NewNode->distance < FiboHeap->Min->distance)
          FiboHeap->Min = NewNode;
      }
 
 // We have one more node in the heap, and it is a tree on the root list
 
      FiboHeap->current_nodes++;
      FiboHeap->current_trees++;
      NewNode->Parent = NULL;
}
void  AddToRootList(FiNd *x,FiHp *fiboheap)
 {
      if (x->marked) fiboheap->marked_nodes--;
      x->marked = false;
      fiboheap->current_nodes--;
      Insert(x,fiboheap);
 }

//in case of consolidation


//<=============================================== Extract Min ===============================================================================>
void print_list(FiNd* listhead){
    
    while(listhead!=NULL){
printf("%d->",listhead->v);
listhead=listhead->Right;
if(listhead==NULL) printf("NULL \n");
                }
    }

void  Union(FiHp *OtherHeap,FiHp *fiboheap)
 {
 FiNd *Min1, *Min2, *Next1, *Next2;
 
      if (OtherHeap == NULL || OtherHeap->Min == NULL) return;
 
 // We join the two circular lists by cutting each list between its 
 // min node and the node after the min.  This code just pulls those
 // nodes into temporary variables so we don't get lost as changes
 // are made.
 
      Min1 = fiboheap->Min;
      Min2 = OtherHeap->Min;
      Next1 = Min1->Right;
      Next2 = Min2->Right;
 
  
      Min1->Right = Next2;
      Next2->Left = Min1;
      Min2->Right = Next1;
      Next1->Left = Min2;
 
 // Choose the new minimum for the heap
  
      if (Min2->distance < Min1->distance)
          fiboheap->Min = Min2;
 
                    
     //NumNodes += OtherHeap->NumNodes;
      //NummarkedNodes += OtherHeap->NummarkedNodes;
      //NumTrees += OtherHeap->NumTrees;
 
     free(OtherHeap);
     OtherHeap=NULL;
 
     }

     void _Link(FiNd *y, FiNd *x,FiHp *fiboheap)
 {
 // Remove node y from root list
 
      if (y->Right != NULL)
      y->Right->Left = y->Left;
      if (y->Left != NULL)
          y->Left->Right = y->Right;
      fiboheap->current_trees--;
 
 // Make node y a singleton circular list with a parent of x
 
      y->Left = y->Right = y;
      y->Parent = x;
 
 // If node x has no children, then list y is its new child list
 
      if (x->Child == NULL){
      x->Child = y;}
   //   printf("%d has child %d \n",x->v,x->Child->v);}
 // Otherwise, node y must be added to node x's child list
 
      else
      {
          y->Left = x->Child;
          y->Right = x->Child->Right;
          x->Child->Right = y;
          y->Right->Left = y;
      }
 
 // Increase the degree of node x because it's now a bigger tree
 
      x->rank ++;
 
 // Node y has just been made a child, so clear its mark
 
      if (y->marked) fiboheap->marked_nodes--;
      y->marked = false;
      return;
 }
     void _Consolidate(FiHp *fiboheap)
 {
 FiNd *x, *y, *w,*temp; // 1+lg(n)
 int Dn = degrees(fiboheap->current_nodes);
 FiNd *A[Dn+1];
 short d;
 int i;
 //printf("Fiboheapmin=%d\n",fiboheap->Min->v);
 //printf("Dn=%d\n",Dn);
 // Initialize the consolidation detection array
 
      for (i=0; i <= Dn; i++)
           A[i] = NULL;
 
 // We need to loop through all elements on root list.
 // When a collision of degree is found, the two trees
 // are consolidated in favor of the one with the lesser
 // element key value.  We first need to break the circle
 // so that we can have a stopping condition (we can't go
 // around until we reach the tree we started with
 // because all root trees are subject to becoming a
 // child during the consolidation).
 
      fiboheap->Min->Left->Right = NULL;
      fiboheap->Min->Left = NULL;
    //  print_list(fiboheap->Min);
      w = fiboheap->Min;
   // printf("Fiboheapmin=%d\n",fiboheap->Min->v);
    //printf("w=%d\n",w->v);
      do {
 //cout << "Top of Consolidate's loop\n";
 //Print(w);
          //print_list(w);
         x = w;
         d = x->rank;
         w = w->Right;
      //   printf("w=%d\n",w->v);

 // We need another loop here because the consolidated result
 // may collide with another large tree on the root list.
    //printf("A[d]=%d d=%d\n",A[d],d);
          while (A[d] != NULL)
         {
             //printf("A[d]=%d d=%d\n",A[d]->v,d);
             //printf("w=%d\n",w->v);
              y = A[d];
          if (y->distance < x->distance){
               temp=x;
               x=y;
               y=temp;
              }
              
              if (w == y) w = y->Right;
              _Link(y, x,fiboheap);
              A[d] = NULL;
              d++;
 //cout << "After a round of Linking\n";
 //Print(x);
     }
    // printf("w=%d\n",w->v);
     A[d] = x;
 
      } while (w != NULL);
 
 // Now we rebuild the root list, find the new minimum,
 // set all root list nodes' parent pointers to NULL and
 // count the number of subtrees.
 
      fiboheap->Min = NULL;
      fiboheap->current_trees = 0;
      for (i = 0; i <= Dn; i++){
           if (A[i] != NULL){
 //              printf("A[%d]=%d ",i,A[i]->v);
      AddToRootList(A[i],fiboheap);}
      }
               
       return;        
 }
 

int  ExtractMin(FiHp *fiboheap)
{
 FiNd *Result;
 FiHp *ChildHeap = NULL;
 
 // Remove minimum node and set fiboheap->Min to next node
 //printf("Fiboheapmin=%d\n",fiboheap->Min->v);
 Result=fiboheap->Min;
      if (Result == NULL)
           return NOTOK;
      //printf("Here1\n");
      fiboheap->Min = Result->Right;
      //printf("Fiboheapmin=%d\n",fiboheap->Min->v);
      Result->Right->Left = Result->Left;
      Result->Left->Right = Result->Right;
      Result->Left = Result->Right = NULL;
      //printf("Fiboheapmin=%d\n",fiboheap->Min->v);
      //printf("Here2\n");
      fiboheap->current_nodes--;
      //fiboheap->nodes[Result->v]=NULL;
      if (Result->marked)
      {
          fiboheap->marked_nodes--;
          Result->marked = false;
      }
      //printf("Here3\n");
      //Result->rank = 0;
 
 // Attach child list of Minimum node to the root list of the heap
 // If there is no child list, then do no work
 
      if (Result->Child == NULL)
      {
          //printf("Fiboheapmin=%d\n",fiboheap->Min);
          //printf("Result=%d\n",Result);
          //printf("Here4\n;");
      if (fiboheap->Min == Result){
          //printf("Fiboheapmin=%d\n",fiboheap->Min->v);
          fiboheap->Min= NULL;}
      }
 
 // If MinRoot==Result then there was only one root tree, so the
 // root list is simply the child list of that node (which is
 // NULL if this is the last node in the list)
 
      else if (fiboheap->Min== Result)
          fiboheap->Min= Result->Child;
 
 // If fiboheap->Min is different, then the child list is pushed into a
 // new temporary heap, which is then merged by Union() onto the
 // root list of this heap.
 
      else 
      {
          //printf("Here6\n;");
          ChildHeap = newFiboHeap(Result->rank);  //we 
          ChildHeap->Min= Result->Child;
      }
      //printf("Fiboheapmin=%d",fiboheap->Min->v);
    //printf("Here9\n;");
 // Complete the disassociation of the Result node from the heap
 
      if (Result->Child != NULL)  Result->Child->Parent = NULL;
      //printf("Here10\n;");
      Result->Child = Result->Parent = NULL;
 
 // If there was a child list, then we now merge it with the
 //  rest of the root list
      //printf("Here11\n;");
      if (ChildHeap!=NULL)
          Union(ChildHeap,fiboheap);
 
 // Consolidate heap to find new minimum and do reorganize work
      //printf("Here12\n;");
      //printf("Fiboheapmin=%d",fiboheap->Min->v);
      if (fiboheap->Min!= NULL){
       //   printf("Here13\n;");
         // printf("Result=%d\n",Result->v);
      _Consolidate(fiboheap);}
    
 // Return the minimum node, which is now disassociated with the heap
 // It has Left, Right, Parent, Child, marked and Degree cleared.
 
      return Result->v;

}

//<============================================================================================================================================>






//<================================================ D E C R E A S E     K E Y==================================================================>


//We use cut to remove child_node from child list and insert it to rootlist if childs priority is smaller than his parent's.

  void _Cut(FiNd *x, FiNd *y,FiHp *fiboheap)
 {
      if (y->Child == x)
          y->Child = x->Right;
      if (y->Child == x) //if 1 child in tree
      y->Child = NULL;
 
      y->rank --;
 
      x->Left->Right = x->Right;    //Adjust right and left sibling of child
      x->Right->Left = x->Left;
 
      AddToRootList(x,fiboheap);   //Child goes to root list
 }

/*

We use cascading cut to also cut marked nodes(child's ancestors) if needed.
  
*/

 int ancestors_cut=0;
 int decrease_keys=0;

 void _CascadingCut(FiNd *y,FiHp *fiboheap)
 {
 FiNd *z = y->Parent;
 
      while (z != NULL)
      {
          if (y->marked == false)
          {
              y->marked = true;
              fiboheap->marked_nodes++;
              z = NULL;
          }
          else
          {
          	   ancestors_cut++;
              _Cut(y, z,fiboheap);
              y = z;
          z = y->Parent;
          }
      }
 }


 int  DecreaseKey(FiNd *theNode, long int  newpriority,FiHp *fiboheap)
 {
     FiNd *theParent;
 
      if (theNode==NULL || theNode->distance < newpriority) //Something went wrong
          return NOTOK;
 
      theNode->distance = newpriority; //change priority
 
      theParent = theNode->Parent;
      if (theParent != NULL && theNode->distance < theParent->distance) 
      {                                                               
          _Cut(theNode, theParent,fiboheap);                               
          _CascadingCut(theParent,fiboheap);                                                       
      }                                                                                                                                                  
                               
      if (theNode->distance < fiboheap->Min->distance) //Change min if 
      	                                    //newpriotity is smaller than min
          fiboheap->Min = theNode;
 
      return OK;
 }



                                                                                       
                                                                                     
//


 




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
int counter=0;
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
      FiHp* fheap;
      fheap=newFiboHeap(V);
      FiNd* newNode;
      int v; 
    // Initialize min heap with all vertices. dist value of all vertices 
	#ifdef INSERT
    		gettimeofday(&ts,NULL);
	#endif  
    for (v = 0; v < V; ++v)
    {
        if(v==src)
        { dist[src]=0; 
           newNode=newFiboNode(src,dist[src]);
          Insert(newNode,fheap);
          //print_list(fheap->Min);
          }
        else{
        dist[v] = INT_MAX;
        //minHeap->array[v] = newMinHeapNode(v, dist[v]);
        //minHeap->pos[v] = v;
        //print_list(fheap->Min);
        newNode=newFiboNode(v,dist[v]);
        Insert(newNode,fheap);
        //printf("Vertex:%d inserted",v);
        }
        fheap->nodes[newNode->v]=newNode;
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
    while (fheap->current_nodes!=0)
    {
    
        // Extract the vertex with minimum distance value
   //     printf("About to Extract");
        //double time;            //variables for timing
        //struct timeval ts,tf;
	#ifdef EXTRACT
        	gettimeofday(&ts,NULL);
	#endif
    #ifdef EXTRACT_FIRST
        	gettimeofday(&ts,NULL);
	#endif
        int u = ExtractMin(fheap);
	#ifdef EXTRACT
     	    gettimeofday(&tf,NULL);
        	time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
        	total_extract_time+=time;
	#endif
    #ifdef EXTRACT_FIRST
        	if(counter==0){
     	    gettimeofday(&tf,NULL);
        	total_extract_time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001; 
        	printf("Time_extractf:%lf:",total_extract_time);
        	exit(0);       	
        	counter++;
        		}
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
            if (fheap->nodes[v]!=NULL && dist[u] != INT_MAX && 
                                          pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
 
             
			   // update distance value in min heap also
		
                decrease_keys++;
		#ifdef DECREASE
         	       gettimeofday(&ts,NULL);	
		#endif
	
                check=DecreaseKey(fheap->nodes[v], dist[v], fheap);
	
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
    #ifdef EXTRACT_FIRST
    printf("total_extract_time=%lf:",total_extract_time);
    #endif
    printf("Vertices:%d:Edges:%d\n",V,edges);
    printf("Decrease keys =%d , Ancestors cut=%d\n",decrease_keys,ancestors_cut);
    //printf("Time to decrease=%lf \n",total_decrease_time);
    //printf("Update time: %lf",total_update_time);

    return 0;
}
