#include <stdio.h>
#include "transaction.h"
#include <pthread.h>
#include <immintrin.h>
#include <omp.h>




void begin_transaction(int retries, omp_lock_t  *lock, count_abort_t *counter)
{
	int num_retries = retries;
	int aborts = 0;

	while(1){
		int status = _xbegin();

  		if(status == _XBEGIN_STARTED){ 		//> Transaction started successfully
    							//> Check if lock is free, else abort explicitly.
    							//> This also adds the lock to the read set so we abort if another thread acquires(writes to) the lock.
   			if(omp_test_lock(lock))
      				_xabort(0xff);

 		}else{					//> Transaction was aborted
    			aborts++;
    							//> Abort reason was...
    			if(status & _XABORT_CONFLICT)	//> data conflict
				counter->conflicts++;
			else if(status & _XABORT_CAPACITY)	//> capacity (transaction read or write set overflow)
       				counter->capacity++;
  			else if(status & _XABORT_EXPLICIT)
				counter->explicits++;
			else if(status & _XABORT_DEBUG)
				counter->debug++;
			else if(status & _XABORT_NESTED)
				counter->nested++;
			
			if(status & _XABORT_RETRY)
				counter->retry++;
	
    			//> If we exceeded the number of retries acquire the lock and move forward to execute the
    			//> critical section. Else retry in speculative mode (e.g. go back to the while loop).
    			if(aborts >= num_retries)
      				omp_set_lock(lock);
    			else
      				continue;
		}
		return;
	}
}

void end_transaction(omp_lock_t  *lock,count_abort_t *counter)
{
	if(_xtest())
    		_xend();
	else
    		omp_unset_lock(lock);

	return;
}


