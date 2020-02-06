#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void setupQueue(struct Queue *queue){
	queue->numOfItems = 0;
	queue->front = NULL;
	queue->rear = NULL;
}

int isEmpty(struct Queue *queue){
	return(queue->front == NULL);
}

void enqueue(Queue *queue, char* value){
	node *node = malloc(sizeof(node));
	node->fullFilePath = value;
	node->next = NULL;
	if(isEmpty(queue) == 1){
		queue->front = node;
		queue->rear = node;
	}
	else{
		queue->rear->next = node;
		queue->rear = node;
	}
	queue->numOfItems++;
}

node* dequeue(Queue *queue){
	if(isEmpty(queue)){
		fprintf(stderr, "Error: Attempted to dequeue empty queue");
		return NULL;
	}
	else{
		node *tmp;
		tmp = queue->front;
		queue->front = queue->front->next;
		queue->numOfItems--;
		return tmp;
	}
}
