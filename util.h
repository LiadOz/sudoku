#ifndef UTIL_H_
#define UTIL_H_
#include "parsing.h"
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
void push(Node** top, int x, int y, int value, int counter);
int pop(Node** top, Board* b);
int top(Node* top);

#define NOT_INT 1
#define NOT_FLOAT 1


int check_if_int(char* s, int* flag);
float check_if_float(char* s, int* flag);
int* set_params_int(Command* cmd, int* flags);
int check_cell_errorness(Board* b, int x, int y, int val);
int out_of_range(Board* b, int* args);
void board_errorness(Board* b);

#endif /* end of include guard: UTIL_H_ */
