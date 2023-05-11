#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
  int count;
  queue_t wait_list;
};

// we want to call preempt disable on every sem function and enable it before it returns because we want our sem functions to be atomic

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
  // allocate and init a semaphore with internal count = count
  // on heap
  preempt_disable();
  sem_t sem = malloc(sizeof(struct semaphore));
  if (sem == NULL){
    return NULL;
  }
  sem->wait_list = queue_create();
  sem->count = count;
  preempt_enable();
  return sem;

  
  //return ptr to semaphore
  // null if failed
  
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
  preempt_disable();
  // deallocate sem
  if (sem == NULL || queue_length(sem->wait_list) > 0){
    return -1;
  }

  queue_destroy(sem->wait_list);
  free(sem);

  preempt_enable();
  return 0;
  //return -1 if sem is null or if other threads still block on sem
  // 0 if success in destroy
}

int sem_down(sem_t sem) // also known as wait() or P()
{
	/* TODO Phase 3 */
  // take sem
  // take rsrc from sem
    // if take unavailable rsrc -> block caller until rsrc available
  // return -1 if sem is null; 0 if sem taken
  preempt_disable();
  if (sem == NULL) {
    return -1;
  }

  // add to wait list then block
  if (sem->count == 0){
    queue_enqueue(sem->wait_list, uthread_current());
    uthread_block();
  }

  // decrement count
  if (sem->count > 0){
    sem->count--;
  }

  preempt_enable();
  
  return 0;
  
  
  
}

int sem_up(sem_t sem) //post
{
	/* TODO Phase 3 */
  // release sem
  // if waiting list associated with sem not empty, releasing rsrc cause
  // 1st thread (ie oldest) in waiting list to be unblocked
  // return -1 if sem null; 0 if successful release
  preempt_disable();
  
  if (sem == NULL){
    return -1;
  }
  
  sem->count++;

  if (queue_length(sem->wait_list) > 0){
    struct uthread_tcb *next_thread;
    queue_dequeue(sem->wait_list, (void*)&next_thread);
    uthread_unblock(next_thread);
  }

  preempt_enable();
  return 0;
}
