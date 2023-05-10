#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

// create new node

struct queue
{
    /* TODO Phase 1 */
    /* didn't typedef cause it was professor's starter code */
    Node *head;
    Node *tail;
    int length;
};
/* Note: queue_t = pointer to struct queue */

queue_t queue_create(void)
{
    /* TODO Phase 1 */
    queue_t new_queue = malloc(sizeof(struct queue));
    if (new_queue == NULL)
    {
        /*ERROR: failure when allocating*/
        return NULL;
    }
    new_queue->length = 0;
    new_queue->head = NULL;
    new_queue->tail = NULL;
    return new_queue;
}

int queue_destroy(queue_t queue)
{
    /* TODO Phase 1 */
    if (queue == NULL || queue->length != 0)
    {
        /*ERROR: queue is NULL or queue's not empty*/
        return -1;
    }
    printf("dbg\n");
    // DO WE HAVE TO DEALLOCATE NODES HERE?
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
    /* TODO Phase 1 */
    Node *new_node = malloc(sizeof(Node));
    if (queue == NULL || data == NULL || new_node == NULL)
    {
        /*ERROR: queue or data is NULL pointer or memory allocation failed*/
        return -1;
    }
    new_node->data = data; // Have to ask TA just to be sure...
    new_node->next = NULL;
    /*
    Two scenerios:
    1. Empty list: new node becomes the head and tail
    2. Non-empty list: put node at the end of the queue and move the tail pointer there
    */
    if (queue->head == NULL)
    {
        queue->head = new_node;
        queue->tail = new_node;
    }
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->length++;
    return 0; /*SUCESS enqueuing*/
}

int queue_dequeue(queue_t queue, void **data)
{
    /* TODO Phase 1 */
    if (queue->head == NULL||queue == NULL || data == NULL || queue->length == 0)
    {
        /*ERROR queue/data is NULL or queue is empty*/
      printf("failed to deq\n");
        return -1;
    }

    // have to dereference ptr bc you pass in an address
    *data = queue->head->data; /*why does it have to be void**?*/
    /*Dequeue the oldest node*/
    Node *previousNode = queue->head;
    queue->head = queue->head->next;

    // if head becomes NULL then q is empty and change tail to NULL
    if (queue->head == NULL)
    {
      queue->tail = NULL;
    }
    
    free(previousNode);
    queue->length--;
    return 0;
}

int queue_delete(queue_t queue, void *data)
{
    /* TODO Phase 1 */
    if (queue == NULL || data == NULL)
        return -1;
    Node *currNode = queue->head;

    while (1)
      {
        if (currNode->data == data)
        {
            /*this is basically comparing 2 addresses -> not sure...
            seems like I have to typecast them but if I do, why do I even need it to be void pointers?*/
            Node *foundNode = currNode;
            currNode->next = currNode->next->next;
            free(foundNode);
            queue->length--;
	    return 0;
        }
        if (currNode->next == NULL)
            return -1;
        currNode = currNode->next;
    }
}

// void (*queue_func_t)(queue_t queue, void *data)
int queue_iterate(queue_t queue, queue_func_t func)
{
    /* TODO Phase 1 */
    if (queue == NULL || func == NULL)
    {
        return -1;
    }
    /* The resulting values should be stored in the queue? */
    Node *currNode = queue->head;
    for (; currNode->next != NULL; currNode = currNode->next)
    {
      (func)(queue, currNode->data);
    }
    return 0;
}

int queue_length(queue_t queue)
{
    /* TODO Phase 1 */
    return queue->length;
}
