#ifndef __SLPQ_H_
#define __SLPQ_H_

#define CACHE_LINE_SIZE 64

//> A node of the id_list
struct id_node {
	unsigned int vertex_id;
	struct id_node *next;
	struct id_node *prev;
};

//> A node of the priority queue
struct pq_node {
	float key;
	int height;
	struct pq_node **next;
	struct pq_node **prev;
	//> Padding for false sharing
	//char padding[CACHE_LINE_SIZE - sizeof(float) - sizeof(unsigned int) - sizeof(int) - 2 * sizeof(struct pq_node **)];
	//char padding[CACHE_LINE_SIZE];
	struct id_node *id_list;
};

//> The priority queue struct
typedef struct {
	struct pq_node *head, *tail;
	unsigned int levelmax;
	unsigned int curr_size;
	unsigned int nvertices;
	struct pq_node **where_in_pq;
	struct id_node **where_in_id_list;
} pq_t;



extern struct pq_node *pq_node_new(float key, unsigned int vertex_id, int toplevel);
extern struct id_node *pq_node_add_id(struct pq_node *node, unsigned int vertex_id);
extern void pq_node_free(struct pq_node *node);
extern struct id_node *pq_node_delete_vertex_id(pq_t *pq, struct pq_node *node, unsigned int vertex_id);
extern pq_t *pq_create(unsigned int nvertices);
extern int get_rand_level(int levelmax);
extern int pq_insert(pq_t *pq, float key, unsigned int vertex_id);
extern int pq_read_min(pq_t *pq, int index);
extern int pq_delete_min(pq_t *pq);
struct id_node *delete_node_with_vertex_id(pq_t *pq, unsigned int vertex_id);
extern void pq_decrease_key(pq_t *pq, unsigned int vertex_id, float new_key);
extern int print(pq_t *pq);
#endif

