#include "board.h"

typedef struct Node {
	int x;
	int y;
	int value;
	int counter;
	struct Node* prev;
} Node;

Node* new_node(int x, int y, int value, int counter);
void change_value(Node* node, int value);
int is_empty(Node* top);
void push(Node** top, int x, int y, int value);
int pop(Node** top, Board* b);
int top(Node* top);