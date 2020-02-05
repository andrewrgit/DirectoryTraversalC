#include <stddef.h>
#include "queue.h"

void setupQueue(struct Queue *queue){
	queue->numOfItems = 0;
	queue->front = NULL;
	queue->rear = NULL;
}

int isEmpty(struct Queue *queue){
	return(queue->front == NULL);
}
