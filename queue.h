#ifndef QUEUE_H_
#define QUEUE_H_

struct node{
	int value;
	struct node *next;
};

struct Queue{
	int numOfItems;
	struct node *front;
	struct node *rear;
};

typedef struct Queue Queue;
typedef struct node node;

void setupQueue(struct Queue *queue);

int isEmpty(struct Queue *queue);

node* dequeue(Queue *queue);

#endif
