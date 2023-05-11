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

void signal_handler(int signum){
  printf("SIGVT RECEIVED!\n");
  uthread_yield(uthread_current());
}

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

//signal handler code is adapted from lecture slides on syscalls
// 100 Hz adapted from https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/setitime.html
void preempt_start(bool preempt)
{
	/* TODO Phase 4 */
  // 1. install a signal handler that receives alarm signals (SIGVTALRM)
  // 2. config timer which will fire alrm (SIGVTALRM) 100 times per sec (100Hz))

  // SIGVTALRM - this signal typically inidcates expiration of a timer that measures
  // CPU time used by current proc. aka virtual alarm time

  // signal handler, which acts as the timer interrupt handler, will force current
  // running thread to yield

  // if preemption enabled, stop should be called before uthread_run returns,
  // once the multithreading phase returns
  // It should restore prev signal action, and restore prev timer configuration
  if (preempt){
    //start preemption
    // configure a timer that must fire a virutal alarm w/ freq 100Hz
    // set up timer handler that forcefully yields currently running thread
    struct sigaction sa;
    sigset_t ss;

    // set up handler
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGVTALRM, &sa, NULL);

    // it_interval is the period between successive timer interrupts. If zero, the alarm will only be sent once.
    // it_value is the period between now and the first timer interrupt. If zero, the alarm is disabled.

    struct itimerval new;
    
    // configure alarm to 100Hz (100 times per sec)
    new.it_value.tv_sec = 0; // set initial time to 0 sec
    
    // bc we want 100 times per sec
    // ms = MILLI-second
    // 1 second = 1000 ms
    // 1000 MILLI-second / 100 = 10 ms
    // that means we want the timer to interrupt every 10 ms
    // usec = MICRO-seconds
    // 1 usec = 1000 ms
    // 10 usec = 10000 ms
    new.it_value.tv_usec = 10000; // will count down from 10ms and then fire SIGVTALARM
    new.it_interval.tv_sec = 0;
    new.it_interval.tv_usec = 10000; // 10 ms
    setitimer(ITIMER_VIRTUAL, &new, NULL);
  // if preempt false then all other function should be ineffective
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
  // stop thread preemption

  // restore previous timer configuration and prev action associated with virt.
  // alarm sigs
}

