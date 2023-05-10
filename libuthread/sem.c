#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
  int count;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
  // allocate and init a semaphore with internal count = count
  // on heap
  
  //return ptr to semaphore
  // null if failed
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
  // deallocate sem

  //return -1 if sem is null or if other threads still block on sem
  // 0 if success in destroy
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
  // take sem

  // take rsrc from sem

  // if take unavailable rsrc -> block caller until rsrc available
  // return -1 if sem is null; 0 if sem taken
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
  // release sem
  // if waiting list associated with sem not empty, releasing rsrc cause
  // 1st thread (ie oldest) in waiting list to be unblocked

  // return 1 if sem null; 0 if successful release
}

