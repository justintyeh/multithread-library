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

//global access for new action and old action
struct sigaction new_action, old_action;
struct itimerval new_timer, old_timer;
sigset_t ss;
bool enable;

// signal handler, which acts as the timer interrupt
// handler, will force current running thread to yield
void signal_handler(int signum){
  signum = signum; // avoid werror unused var
  printf("SIGVT RECEIVED!\n");
  uthread_yield();
}

void preempt_disable(void)
{
  // disable preemption so block the signals
  if (enable){
    sigemptyset(&ss);
    sigaddset(&ss, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &ss, NULL);
  }
}

void preempt_enable(void)
{
  // enable preemption so unblock the signals
  if (enable){
    sigemptyset(&ss);
    sigaddset(&ss, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &ss, NULL);
  }
}

// 100 Hz set up is adapted from
// https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/setitime.html
void preempt_start(bool preempt)
{
  // if preempt false then all other function should be ineffective
  enable = preempt;
  // 1. install a signal handler that receives alarm signals (SIGVTALRM)
  // 2. config timer which will fire alrm (SIGVTALRM) 100 times per sec (100Hz))

  // SIGVTALRM - this signal typically inidcates expiration of a timer that measures
  // CPU time used by current proc. aka virtual alarm time

  // start preemption
  if (enable){
    // set up signal handler that forcefully yields currently running thread
    new_action.sa_handler = signal_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGVTALRM, &new_action, &old_action);
    
    // it_interval is the period between successive timer interrupts. If zero, the alarm will only be sent once.
    // it_value is the period between now and the first timer interrupt. If zero, the alarm is disabled.

    // configure alarm to fire signal 100Hz (100 times per sec)
    new_timer.it_value.tv_sec = 0; // set initial time to 0 sec
    
    // bc we want 100 times per sec
    // ms = MILLI-second
    // 1 second = 1000 ms
    // 1000 MILLI-second / 100 = 10 ms
    // that means we want the timer to interrupt every 10 ms
    // usec = MICRO-seconds
    // 1 usec = 1000 ms
    // 10 usec = 10000 ms
    new_timer.it_value.tv_usec = 10000; // will count down from 10ms and then fire SIGVTALARM
    new_timer.it_interval.tv_sec = 0;
    new_timer.it_interval.tv_usec = 10000; // 10 ms
    setitimer(ITIMER_VIRTUAL, &new_timer, NULL);
  }
}

void preempt_stop(void)
{
  // stop thread preemption
  // restore previous timer configuration
  // and prev action associated with virt.
  if (enable){
    sigaction(SIGVTALRM, &old_action, NULL);
  }
}

