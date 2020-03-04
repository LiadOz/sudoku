#include <stdio.h>
#include "stack.h"
#include "board.h"
#include "printing.h"


/*Return new node of the next empty cell*/
Node* next_empty_cell(Board* b, int x, int y, int counter) {
	Node* node = NULL;
	int i, j;
	for (i = x; i < b->size; i++) {
		for (j = 0; j < b->size; j++) {
			if (b->state[i][j] == 0) {
				node = new_node(i, j, 1, counter);
				return node;
			}
		}
	}
	return node;
}

/*Return new node of the first empty cell*/
Node* first_empty_cell(Board* b) {
	return next_empty_cell(b, 0, 0, 0);
}

int num_of_solutions(Board* b) {
	int x, y;
	Node* node = first_empty_cell(b);
	Node* temp = NULL;
	int counter = 0;

	while (1) {
		if (!node) {
			return pop(&node, b);
		}

		while(node->value <= b->size) {
			x = node->x; y = node->y;
			if (valid_set_value(b, x, y, node->value)) {
				b->state[node->x][node->y] = node->value;
				temp = next_empty_cell(b, node->x, node->y, node->counter);
				if (!temp) {
					node->counter++;
					counter = pop(&node, b);
					if (!node) {
						return counter;
					}
					else {
						node->counter = counter;
						node->value++;
					}
				}
				else {
					temp->prev = node;
					node = temp;
				}
			}
			else {
				node->value++;
			}
		}

		counter = pop(&node, b);
		if (!node) {
			return counter;
		}
		else {
			node->counter = counter;
			node->value++;
		}
	}
}
