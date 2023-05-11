#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"


// queue for READY threads
queue_t q;

enum states{
  RUNNING = 1,
  READY = 0,
  BLOCKED = -1,
  ZOMBIE = -2
};

// thread specific information
struct uthread_tcb {
  /* TODO Phase 2 */
  ucontext_t *context;// set of registers
  void *SP;
  int state;

};

// declare global idle and initial thread to keep track of them
struct uthread_tcb *idle_thread;
struct uthread_tcb *initial_thread;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
  //printf("return curr thread\n");
  return initial_thread;
  
}

void uthread_yield(void)
{
  //printf("yield\n");
  // change current thread from RUNNING -> READY and add to queue
  struct uthread_tcb *prev_thread = uthread_current();
  if (prev_thread->state == RUNNING){
    prev_thread->state = READY;
    queue_enqueue(q, prev_thread);
  }

  // get the next thread from queue and change status to RUNNING
  struct uthread_tcb *next_thread;
  queue_dequeue(q, (void*)&next_thread);
  next_thread->state = RUNNING;

  // keep track of new initial thread
  initial_thread = next_thread;

  // context switch previous thread and next thread
  uthread_ctx_switch(prev_thread->context, next_thread->context); 
    
}

void uthread_exit(void)
{
  /* TODO Phase 2 */
  //printf("exit\n");
    // exit the thread so change to ZOMBIE state
  initial_thread->state = ZOMBIE;
  
  // stop running this thread
  uthread_yield();
  
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
  //printf("create\n");
  // create new thread
  struct uthread_tcb *new_thread = malloc(sizeof(struct uthread_tcb));
  new_thread->context = malloc(sizeof(ucontext_t));

  // set state to READY
  new_thread->state = READY;

  // allocate stack with function
  new_thread->SP = uthread_ctx_alloc_stack();


  // initialize context
  uthread_ctx_init(new_thread->context, new_thread->SP, func, arg); // -1 if fails

  // add thread to queue
  queue_enqueue(q, new_thread);
  return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
  //printf("run\n");
  // skip preempt for now
  if (preempt){
    ;
  }
    
  //printf("run\n");
  q = queue_create();
  
  // initialize new thread
  struct uthread_tcb *new_thread = malloc(sizeof(struct uthread_tcb));
  new_thread->context = malloc(sizeof(ucontext_t));

  // idle thread will now be running
  new_thread->state = RUNNING;
  initial_thread = new_thread;
  idle_thread = new_thread;


  uthread_create(func, arg); // returns -1 if failed

  // keep yielding current thread until you reach the idle thread 
  while (queue_length(q) > 0){
    uthread_yield();
  }

  // free the memory of initial thread bc they are done running
  free(initial_thread->context);
  free(initial_thread->SP);

  // no need to free idle thread bc we need it to run
  // and the program will finish running when idle thread is done
  // which will automatically free the memory

  // If preemption was enabled, the function preempt_stop() should be called before uthread_run() returns, once the multithreading phase of the application ends. It should restore the previous signal action, and restore the previous timer configuration.
  if (preempt){
    preempt_stop();
  }
  
  return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
  //printf("block\n");
  struct uthread_tcb *current = uthread_current();
  current->state = BLOCKED;
  uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
  //printf("unblock\n");
  uthread->state = READY;
  queue_enqueue(q, uthread);
}
