#ifndef __UTIL_H__
#define __UTIL_H_

#include <unistd.h>

#define MAXCACHELINE 128
#define _MIN(A,B) ((A)<(B)?(A):(B))
#define _MAX(A,B) ((A)>(B)?(A):(B))
#define _MAXE(A,B) ((A)>=(B)?(A):(B))
								
extern void *malloc_aligned(size_t size, size_t alignment);
extern void *malloc_safe(size_t size);

#ifdef NUMA
extern void* numa_alloc_onnode_safe(size_t size, int node);
#endif

extern ssize_t buffered_read(int fd, unsigned char *ptr);
extern ssize_t readline(int fd, char *vptr, size_t maxlen);

#endif
