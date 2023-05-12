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

// we want to call preempt disable on every sem function
// and enable it before it returns because we want our sem
// functions to be atomic

// initialize semaphore
// success: return pointer to semaphore
// fail: return NULL
sem_t sem_create(size_t count)
{
    preempt_disable();
    sem_t sem = malloc(sizeof(struct semaphore));
    if (sem == NULL) {
        return NULL;
    }
    sem->wait_list = queue_create();
    sem->count = count;
    preempt_enable();
    return sem;
}

// return -1 if sem is null or if other threads still block on sem
// if successfully destroyed return 0
int sem_destroy(sem_t sem)
{
    preempt_disable();

    if (sem == NULL || queue_length(sem->wait_list) > 0) {
        return -1;
    }

    queue_destroy(sem->wait_list);
    free(sem);

    preempt_enable();
    return 0;
}

// fail: -1
// sucess: 0
int sem_down(sem_t sem)
{
    // take resource from sem
    // if resource count == 0 then add thread to wait list and call block

    preempt_disable();
    if (sem == NULL) {
        return -1;
    }

    // add to wait list then block
    if (sem->count == 0) {
        queue_enqueue(sem->wait_list, uthread_current());
        uthread_block();
    }

    // decrement count
    if (sem->count > 0) {
        sem->count--;
    }

    preempt_enable();
    return 0;
}




// return -1 if sem null; 0 if successful release
int sem_up(sem_t sem) //post
{
    preempt_disable();
    if (sem == NULL) {
        return -1;
    }

    // release resource so increase count
    sem->count++;

    // if there is a thread in the wait list unblock it so it can run
    if (queue_length(sem->wait_list) > 0) {
        struct uthread_tcb* next_thread;
        queue_dequeue(sem->wait_list, (void*)&next_thread);
        uthread_unblock(next_thread);
    }

    preempt_enable();
    return 0;
}
