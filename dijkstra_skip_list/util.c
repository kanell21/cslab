/*
 * Utility functions (memory allocation, file I/O, etc.)
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef NUMA
#include <numa.h>
#endif

#include "util.h"



/*
 * MEMORY ALLOCATION
 *
 */  

/* 
 * Allocate 'size' bytes, aligned at an address that
 * is a multiple of 'alignment'
 */
void* malloc_aligned(size_t size, size_t alignment)
{
	void *p;

	if(!(p = malloc(size + alignment-1))) {
		perror("Error in memory allocation!");
		exit(EXIT_FAILURE);
	}

	//Perform the actual alignment: 'alignment' must be a power of 2 
	p = (void *) (((unsigned long)p + alignment - 1) & ~(alignment - 1));
										
	return p;
}


void* malloc_safe(size_t size)
{
	void *p;

	if(!(p = malloc(size))) {
		perror("Error in memory allocation!");
		exit(EXIT_FAILURE);
	}

	return p;
}

#ifdef NUMA
void* numa_alloc_onnode_safe(size_t size, int node)
{
	void *p;
	char error[50];

	/*sprintf(error, "Error in allocating memory on node %d\n", node);*/


	if(!(p = numa_alloc_onnode(size, node))) {
		numa_error("Error in allocating memory");
		exit(EXIT_FAILURE);
	}

	return p;
}

#endif



/*
 * FILE I/O
 *
 */ 


#define READ_BUF_SIZE 4096
/*
 *  Diabazei mexri READ_BUF_SIZE xarakthres th fora kai tous epistrefei enan enan.
 *  
 *  fd: O file descriptor ap' opou diabazoume tous xarakthres
 *  ptr: O buffer pou apothikeuoume tous xarakthres, enan th fora.
 *  
 *  Epistrefei 1 se periptwsh pou diabase ena xarakthra swsta, 0 se periptwsh EOF
 *  kai -1 se periptwsh lathous.
 */
ssize_t buffered_read(int fd, unsigned char *ptr)
{
	static int read_cnt = 0;
	static unsigned char *read_ptr;
	static unsigned char read_buf[READ_BUF_SIZE];

	/*
	 *  Kathe fora pou kanoume read o read_cnt ginetai mexri READ_BUF_SIZE kai etsi
	 * stis epomenes klhseis ths den mpainoume sto loop.
	 */
	if (read_cnt <= 0) {
again:
		if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return (-1);
		} else if (read_cnt == 0)       /* Periptwsh EOF */
			return (0);
	
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;

	return(1);
}


/*
 *  Diabazei mia grammh keimenou apo enan file descriptor.
 *  fd: O descriptor ap' opou diabazoume tous xarakthres
 *  vptr: O buffer pou apothikeuoume to apotelesma
 *  maxlen: Megisto mhkos ths grammhs pou diabazoume (sthn
 *  pragmatikothta maxlen-1 kathws xreiazetai na apothikeutei
 *  kai to teliko 0).
 *  
 *  Epistrefei ton arihtmo bytes pou diabasthkan (ews maxline).
 *  
 *  Kanei xrhsh ths buffered_read gia kalyterh apodosh
 *  
 */
ssize_t readline(int fd, char *vptr, size_t maxlen)
{
	ssize_t n, rc;
	unsigned char c, *ptr;
	
	ptr = (unsigned char *)vptr;
	for (n=1; n<maxlen; n++) {
		if ((rc=buffered_read(fd,&c)) == 1) {
			*ptr++ = c;
			if (c == '\n')  /* An einai \n den diabazoume allo */
					break;
		} else if (rc == 0) {
			if (n == 1)                     /* EOF xwris na diabasoume tipota */
				return (0);
			else
				break;                  /* EOF exontas hdh diabasei kati */
		} else
			return (-1);            /* Lathos */
	}

	if (*(ptr-1) == '\n')             
		*(ptr-1) = '\0';
	
	return (n);
}

