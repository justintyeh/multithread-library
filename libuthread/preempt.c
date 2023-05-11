#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

void preempt_disable(void)
{
	/* TODO Phase 4 */
  // disable preemption
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
  // enable preemption
}

void preempt_start(bool preempt)
{
	/* TODO Phase 4 */
  if (preempt){
    //start preemption
    // configure a timer that must fire a virutal alarm w/ freq 100Hz
    // set up timer handler that forcefully yields currently running thread
  }
  // if preempt false then all other function should be ineffective
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
  // stop thread preemption

  // restore previous timer configuration and prev action associated with virt.
  // alarm sigs
}

