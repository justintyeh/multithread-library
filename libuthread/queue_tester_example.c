
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
	int qlen = queue_length(q);
	TEST_ASSERT(0 == qlen);
}

// eq and dq multiple
void mtest(void)
{
  queue_t q;
  fprintf(stderr, "*** TEST mtest ***\n");

  q = queue_create();

  int a = 1;
  queue_enqueue(q, &a);
  
  int b = 2;
  queue_enqueue(q, &b);
  
  int c = 3;
  queue_enqueue(q, &c);
  
  int d = 4;
  queue_enqueue(q, &d);

  TEST_ASSERT(queue_length(q) == 4);

  int *ptr;
  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &a);

  queue_dequeue(q, (void**)&ptr);
  TEST_ASSERT(ptr == &b);

  TEST_ASSERT(queue_length(q) == 2);
}

void destroy(void){
  queue_t q;
  fprintf(stderr, "*** TEST destroy ***\n");
  q = queue_create();
  int x = queue_destroy(q);
  TEST_ASSERT(0 == x);
}

void delete(void){
  queue_t q;
  fprintf(stderr, "*** TEST delete ***\n");
  
  q = queue_create();

  int a = 1;
  queue_enqueue(q, &a);
  
  int b = 2;
  queue_enqueue(q, &b);
  
  int c = 3;
  queue_enqueue(q, &c);
  
  int d = 4;
  queue_enqueue(q, &d);

  int x = queue_delete(q, &c);

  TEST_ASSERT(0 == x);

}

int main(void)
{
	test_create();
	test_queue_simple();
	mtest();
	destroy();
	delete();
	return 0;
}
