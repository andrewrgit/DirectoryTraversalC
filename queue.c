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

void enqueue(Queue *queue, int value){
	node *node = malloc(sizeof(node));
	node->value = value;
	node->next = NULL;
	if(isEmpty(queue)){
		queue->front = node;
	}
	else{
		node->next = queue->front;
		queue->front = node;
		queue->numOfItems++;
	}
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
