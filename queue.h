#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct QueueNode QueueNode;
typedef struct Queue Queue;


typedef struct client {
    int id;
    int waiting_time;
    int serving_time;
    bool is_served;

}Client;

struct QueueNode // Allocated dynamically
{
    Client *client; // For generic data
    QueueNode *next;
};

struct Queue
{
    QueueNode *head;
    QueueNode *tail;
    int size;
};

Queue queue_new();
bool is_empty(Queue queue);
void *enqueue(Queue *queue, Client *client); // Returns NULL if failed, void *data otherwise
void *dequeue(Queue *queue);             // Returns NULL if failed, void *data otherwise
void *peek(Queue queue);                 // Returns NULL if failed, void *data otherwise
void queue_clear(Queue *queue);

#endif