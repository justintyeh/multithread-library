/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"
int set;
int set2;

void thread3(void *arg)
{
	(void)arg;
	printf("thread3\n");
	set2 = 1;
}

void thread2(void *arg)
{
	(void)arg;
	// uthread_yield();
	set2 = 0;
	printf("thread2\n");
	uthread_create(thread3, NULL);
	while (1)
	{
		if (set2 == 1)
		{
			set = 1;
			break;
		}
	}
	printf("finished thread2\n");
}

void thread1(void *arg)
{
	(void)arg;
	set = 0;
	// uthread_yield();
	printf("thread1\n");
	uthread_create(thread2, NULL);
	while (1)
	{
		if (set == 1)
			break;
	}
	printf("finished thread1\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}
/*
Results should be...
thread1
-> enter while loop in thread 1
thread2
-> enter while loop in thread 2
thread3
-> finished thread 2
-> finished thread 1

*/