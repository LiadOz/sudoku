#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "board.h"

Node* new_node(int x, int y, int value, int counter) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->x = x;
	node->y = y;
	node->value = value;
	node->counter = counter;
	node->prev = NULL;
	return node;
}

void change_value(Node* node, int value) {
	node->value = value;
}

int is_empty(Node* top) {
	return !top;
}

void push(Node** top, int x, int y, int value) {
	Node* node = new_node(x, y, value, 0);
	node->prev = *top;
	*top = node;
}

int pop(Node** top, Board* b) {
	Node* temp;
    int counter;
	if (is_empty(*top)) {
		return 0;
	}
	b->state[(*top)->x][(*top)->y] = 0;
	temp = *top;
	*top = (*top)->prev;
    counter = temp->counter;
    free(temp);
	return counter;
}

int top(Node* top) {
	if (is_empty(top)) {
		return 0;
	}
	return top->value;
}
