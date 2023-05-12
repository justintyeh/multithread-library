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
  ucontext_t *context;// set of registers
  void *SP; // stack pointers
  int state;

};

// declare global idle and initial thread to keep track of them
struct uthread_tcb *idle_thread;
struct uthread_tcb *initial_thread;

struct uthread_tcb *uthread_current(void)
{
  return initial_thread;
}

void uthread_yield(void)
{
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
  // exit the thread so change to ZOMBIE state
  initial_thread->state = ZOMBIE;
  
  // stop running this thread
  uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
  // create new thread
  struct uthread_tcb *new_thread = malloc(sizeof(struct uthread_tcb));

  // check if memory allocated
  if (new_thread == NULL) {
    return -1;
  }
  new_thread->context = malloc(sizeof(ucontext_t));
  if (new_thread->context == NULL){
    return -1;
  }

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
  // start preemption if preempt is true
  if (preempt){
    preempt_start(preempt);
  }

  q = queue_create();
  struct uthread_tcb *new_thread = malloc(sizeof(struct uthread_tcb));

  // check if memory is allocated
  if (new_thread == NULL){
    return -1;
  }
  new_thread->context = malloc(sizeof(ucontext_t));
  if (new_thread->context == NULL){
    return -1;
  }
  
  // idle thread will now be running and save them to the global variables
  new_thread->state = RUNNING;
  initial_thread = new_thread;
  idle_thread = new_thread;

  // create new thread if fails return -1
  if (uthread_create(func, arg) == -1){
    return -1;
  }

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

  // If preemption was enabled, the function preempt_stop() should
  // be called before uthread_run() returns, once the multithreading
  // phase of the application ends. It should restore the previous
  // signal action, and restore the previous timer configuration.
  if (preempt){
    preempt_stop();
  }
  
  return 0;
}

// change current thread state to blocked and yield
void uthread_block(void)
{
  struct uthread_tcb *current = uthread_current();
  current->state = BLOCKED;
  uthread_yield();
}

// change thread state to READY and add the thread queue
void uthread_unblock(struct uthread_tcb *uthread)
{
  uthread->state = READY;
  queue_enqueue(q, uthread);
}
