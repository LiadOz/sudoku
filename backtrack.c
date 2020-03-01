#include "stack.h"
#include "board.h"

/*Return new node of the next empty cell*/
Node* next_empty_cell(Board* b, int x, int y, int counter) {
	int stop = 0;
	int i;
	int j;
	for (i = x; i < b->height && !stop; i++) {
		for (j = y; j < b->width && !stop; j++) {
			if (b->state[i][j] == 0) {
				stop = 1;
			}
		}
	}
	if (i == x && j == y && stop == 0) {
		return NULL;
	}
	Node* node = new_node(i, j, 1, counter);
	return node;
}

/*Return new node of the first empty cell*/
Node* first_empty_cell(Board* b) {
	return next_empty_cell(b, 0, 0, 0);
}

int num_of_solutions(Board* b) {
	int x;
	int y;
	Node** node;
	*node = first_empty_cell(b);

	while (1) {
		/*stopping condition*/
		if (*node == NULL) {
			return pop(node, b);
		}

		x = (*node)->x;
		y = (*node)->y;

		while((*node)->value <= b->size) {
			if (valid_set_value(b, x, y, (*node)->value)) {
				b->state[x][y] = (*node)->value;
				Node* temp = next_empty_cell(b, x, y, (*node)->counter);
				if (temp == NULL) {
					(*node)->counter++;
					break;
				}
				temp->prev = *node;
				*node = temp;

			}
			else {
				(*node)->value++;
			}
		}

		Node* temp = (*node)->prev;
		if (temp == NULL) {
			return pop(node, b);
		}
		else {
			temp->counter = pop(node, b);
			*node = temp;
		}
	}
}