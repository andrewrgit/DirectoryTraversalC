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

void setupQueue(struct Queue *queue);

int isEmpty(struct Queue *queue);

typedef struct Queue Queue;
typedef struct node node;
#endif
