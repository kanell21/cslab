#ifndef __TRANSACTION_H_
#define __TRANSACTION_H_

#include <pthread.h>
#include <immintrin.h>
#include <omp.h>
typedef struct count_abort_st {
	unsigned int conflicts;
	unsigned int capacity;
	unsigned int explicits;
	unsigned int nested;
	unsigned int debug;
	unsigned int retry;
}count_abort_t;


extern void begin_transaction(int retries, omp_lock_t *fallback_lock, count_abort_t *counter);
extern void end_transaction(omp_lock_t *fallback_lock, count_abort_t *counter);
#endif
