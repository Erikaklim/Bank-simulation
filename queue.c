#include <stdlib.h>
#include "queue.h"


Queue queue_new()
{
    Queue newQueue = {NULL, NULL, 0};
    return newQueue;
}

bool is_empty(Queue queue)
{
    return queue.size == 0;
}

void *enqueue(Queue *queue, Client *client)
{
    QueueNode *newTail = malloc(sizeof(QueueNode));

    if (newTail == NULL)
        return NULL;

    newTail->client = client;
    newTail->next = NULL;
    queue->size++;

    if (queue->tail == NULL)
    {
        queue->tail = newTail;
        queue->head = queue->tail;
    }
    else
    {
        queue->tail->next = newTail;
        queue->tail = newTail;
    }

    return client;
}

void *dequeue(Queue *queue)
{
    if (is_empty(*queue))
        return NULL;

    QueueNode *oldHead = queue->head;
    queue->head = queue->head->next;

    queue->size--;

    if (queue->head == NULL) // Removed the last item
        queue->tail = NULL;

    void *client = oldHead->client;

    free(oldHead);

    return client;
}

void *peek(Queue queue)
{
    if (is_empty(queue))
        return NULL;

    return queue.head->client;
}

void queue_clear(Queue *queue)
{
    while (!is_empty(*queue))
    {
        dequeue(queue);
    }
}

// void dequeueAndPrint(Queue *q)
// {
//     printf("Size is: %d\n", q->size);
//     for (int i = 0; !is_empty(*q); i++)
//     {
//         double *item = dequeue(q);
//         printf("%d: %f\n", i, *item);
//     }
// }
