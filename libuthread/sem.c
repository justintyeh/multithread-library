#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
  int count;
  queue_t wait_list;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
  // allocate and init a semaphore with internal count = count
  // on heap

  sem_t sem = malloc(sizeof(struct semaphore));
  if (sem == NULL){
    return NULL;
  }
  sem->wait_list = queue_create();
  sem->count = count;
  return sem;

  
  //return ptr to semaphore
  // null if failed
  
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
  // deallocate sem
  if (sem == NULL || queue_length(sem->wait_list) > 0){
    return -1;
  }

  free(sem->wait_list);
  free(sem);

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
  if (sem == NULL) {
    return -1;
  }

  // add to wait list then block
  if (sem->count == 0){
    struct uthread_tcb *current_thread = uthread_current();
    queue_enqueue(sem->wait_list, current_thread);
    uthread_block();
  }

  // decrement count
  if (sem->count > 0){
    sem->count = sem->count--;
  }

  return 0;
  
  
  
}

int sem_up(sem_t sem) //post
{
	/* TODO Phase 3 */
  // release sem
  // if waiting list associated with sem not empty, releasing rsrc cause
  // 1st thread (ie oldest) in waiting list to be unblocked
  // return 1 if sem null; 0 if successful release
  if (sem == NULL){
    return 1;
  }
  
  sem->count = sem->count++;
  //if (sem->count == 0 && queue_length(sem->wait_list) > 0){
    struct uthread_tcb *next_thread;
    queue_dequeue(sem->wait_list, (void*)&next_thread);
    uthread_unblock(next_thread);
    // }
  return 0;
}

