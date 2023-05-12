# USER-LEVEL THREAD LIBRARY

## Summary 
This is the implementation of a `basic user-level thread library` for Linux.
The library provides a complete interface for applications to crate an run
independent threads concurrently.

Main functionalities provided by the library:
1. Create new threads
2. Schedule the execution of threads in a round-robin fashion
3. Enable/ disable preemptive, in other words, provide an interrupt-based
scheduler.
4. Provide a thread synchronization API - semaphores.

## Implementation
The implementation of this project follows 3 main steps:

1. Creating a FIFO queue.

The queue is the underlying data structure that enables us to create the main
functionalities provided by the program. It should run all operations (apart
from the delete and iterate functions) in O(1) time. With that being said, a
linked list was used to implement this queue. However, when implementing the 
linked list, 2 pointers: one to the head of the linked list, or the first node 
and the other to the end of the linked list, or the last node. This will enable 
dequeue and enqueue to both run in O(1) time.

2. Creating the unthread API.

The unthread API allows users to run a process with one or more threads 
(uthread_run function), create one or more threads (uthread_create function), 
allow users to specify when a certain thread yields its execution 
(uthread_yield) or when a thread exits its execution (uthread_exit). 

It is implemented with a queue of "struct threads_tcb". Each of these structs 
contain: (1) pointer to it's stack (2) it's state (running, blocked, ready, 
zombie) (3) pointer to a backup of CPU registers.

These "struct threads_tcb" are placed in a queue. If a thread is created, it 
will added to the end of the queue. Each time a thread yields its execution, 
yet its status is "running", we will add it to the end of the queue (enqueue) 
and remove/dequeue it from the front of the queue. In the main thread, we have 
a while loop active as long as the queue still has elements in it. This while 
loop yields the main thread for every iteration. 

With that being said, once a thread is created, it will be added into the queue 
and will run until it yields or terminates. If it yields, we'll add it to the 
back of the queue. The next node of the queue will always be the main thread, 
but it will yield right away and will be added to the end of the queue. This 
will allowing the main thread to monitor the switching between threads. And the 
next thread on the queue can run.

3. Creating the Semaphore API

We implement the sephamore API with a "struct semaphore". "struct semaphore"
includes an int to keep track of its internal count, or how many resources it 
has, and a pointer to an instance of "struct queue" called the wait list, to 
keep track of the number of threads waiting to access it.

If a thread tries to access a resource (using sem_down function) if the 
semaphore's internal count is > 0 then it is able to use one of those resources.
On the other hand, if the internal count is already 0, then the thread will be 
blocked and added onto the waiting list for that semaphore (we enqueue onto the 
back of the queue).

If a thread releases a resource (not use it anymore). This will increase the 
emaphore's internal count by 1 and the next thread in the waiting list  will be 
unblocked (we dequeue the queue from the front).

4. Implementation of Preemption

We set up a signal handler for the SIGVTALRM signal and an alarm clock that 
runs 100 times per second. We set up the the signal handler such that every 
time SIGVTALRM is raised, we'd yield the current thread and move on the the 
next thread in the queue. 

### Testing preemption

We first initialized 2 global variables: set and set2 of type int.
We tested the preemption by running a main thread , creating another thread 
inside it called "thread1" with preemption enabled. Once thread1 runs, it 
prints out "thread1", sets "set" to 0 and creates another thread, named 
"thread2". It then enters an infite while loop with the only option of breaking 
out when "set" is equal to 1. This infite loop will use up a lot of processing 
power. 

If preemption works, when this happens, it will yield thread1 (move thread 1 to 
the end of the queue of threads waiting to run) and run thread2.

Once thread2 runs, it prints out "thread2", sets "set2" to 0, and creates 
another thread, named "thread3". It then enters an infite while loop with the 
only option of breaking out when "set2" is equal to 1. Before it breaks, it 
will set "set" to be 1/ This infite loop will use up a lot of processing power.

If preemption works, when this happens, it will yield thread1 (move thread 1 to 
the end of the queue of threads waiting to run) and run thread2. 

Once thread3 runs, it prints out "thread3", sets "set2" == 1 and terminates.

On the queue, it is now infact thread1's turn again, but since it hasn't been 
able to break out of the while loop yet (due to "set" not being equal to 1). It 
will eventually be yielded again when there's a signal.

Then, it will be thread2's turn. Thread2, now that "set2" == 1 will be able to 
break out the infinite loop, print out "finished thread2", and set "set" == 1. 

Now that  "set" == 1, thread1 can break out of the while loop and print out 
"finished thread1".

The order of statements printed out will then be...
thread1, thread2, thread3, finished thread2, finished thread1

`Conclusion`: This testing program is effective to test whether or not our 
preempt functionality was properly implemented becaue without preemption 
working, we would never be able to ensure that we'd get the printed out states,
ents in that exact order. In fact, thread1 will be stuck in the while loop 
forever as "set" will have never been to 1. This is because, without preemption 
working properly, we'd never have moved on to thread2 but stay in thread1 which 
is stuck in a while loop.
