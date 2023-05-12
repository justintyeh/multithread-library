#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)             \
  do                                    \
  {                                     \
    printf("ASSERT: " #assert " ... "); \
    if (assert)                         \
    {                                   \
      printf("PASS\n");                 \
    }                                   \
    else                                \
    {                                   \
      printf("FAIL\n");                 \
      exit(1);                          \
    }                                   \
  } while (0)

/* Create */
void test_create(void)
{
    fprintf(stderr, "*** TEST create ***\n");

    TEST_ASSERT(queue_create() != NULL);
}
void print(void) {

}
/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
    int data = 3, * ptr;
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
void test_multiple(void)
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

    int* ptr;
    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(ptr == &a);

    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(ptr == &b);

    TEST_ASSERT(queue_length(q) == 2);
}

void test_destroy(void)
{
    queue_t q;
    fprintf(stderr, "*** TEST destroy ***\n");
    q = queue_create();
    int x = queue_destroy(q);
    TEST_ASSERT(0 == x);
}

void test_delete(void)
{
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

    TEST_ASSERT(-1 == queue_destroy(q));

    int* p1; //, *p2, *p3;
    queue_dequeue(q, (void**)&p1);
    //  queue_dequeue(q,(void**)&p2);
    // queue_dequeue(q,(void**)&p3);
    TEST_ASSERT(0 == queue_length(q));
}

void test_delete2(void) {
    queue_t q;
    fprintf(stderr, "*** TEST delete2 ***\n");

    q = queue_create();
    int a = 10;
    queue_enqueue(q, &a);

    int b = 20;
    queue_enqueue(q, &b);

    int c = 30;
    queue_enqueue(q, &c);

    int d = 40;
    queue_enqueue(q, &d);

    queue_enqueue(q, &d);

    queue_delete(q, &d);
    TEST_ASSERT(queue_length(q) == 4);

    queue_delete(q, &d);
    TEST_ASSERT(queue_length(q) == 3);
};

void test_dequeue(void) {
    queue_t q;
    fprintf(stderr, "*** TEST dequeue ***\n");

    q = queue_create();
    int a = 10;
    queue_enqueue(q, &a);

    int b = 20;
    queue_enqueue(q, &b);

    int c = 30;
    queue_enqueue(q, &c);
    queue_enqueue(q, &c);

    int* ptr;
    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(queue_length(q) == 3);
    TEST_ASSERT(ptr == &a);

    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(queue_length(q) == 2);
    TEST_ASSERT(ptr == &b);

    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(queue_length(q) == 1);
    TEST_ASSERT(ptr == &c);

    queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(queue_length(q) == 1);
    TEST_ASSERT(ptr == &c);

    TEST_ASSERT(queue_length(q) == 0);
};
void test_destroy2(void) {
    queue_t q;
    fprintf(stderr, "*** TEST destroy2 ***\n");

    q = queue_create();
    int a = 10;
    queue_enqueue(q, &a); //queue is not empty return -1
    int x = queue_destroy(q);
    TEST_ASSERT(x == -1);
};

void test_length(void) {
    queue_t q;
    fprintf(stderr, "*** TEST length ***\n");

    q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
    TEST_ASSERT(queue_length(NULL) == -1); //ERROR with test_length
};
void test_delete_error(void) {
    queue_t q;
    int a = 10;
    queue_enqueue(q, &a);

    int b = 20;
    queue_enqueue(q, &b);

    int c = 30;
    queue_enqueue(q, &c);

    int d = 30;
    TEST_ASSERT(queue_delete(q, NULL) == -1);
    TEST_ASSERT(queue_delete(q, &d) == -1);
};



int main(void)
{
    test_create();
    test_queue_simple();
    test_multiple();
    test_destroy();
    test_destroy2();
    test_delete();
    test_delete2();
    test_dequeue();
    test_length();
    test_delete_error();
    return 0;
}
