/*
* This module contains all util functions that supports the other moduls.
* ******      Functions:      ******
* printBoard - prints the board to the screen.
* check_if_int - Returns NOT_INT if is not int, if it is int it returns the number.
* check_if_float - Returns NOT_FLOAT if is not float, if it is int it returns the number.
* set_params_int -Check if the parameters are ints, if those are ints' it returns an array of them.
* out_of_range - Returns 1 if 1 of the parameters is out of range, 0 otherwise.
* board_errorness - Returns 1 if cell is errorness, 0 otherwise.
* save_board -
* read_file - Reads a file and creates a new board according to it.
*/

#ifndef UTIL_H_
#define UTIL_H_
#include "cli.h"
#include "board.h"


/* Represents a node in a stack which is used in bactrack.c */
typedef struct Node {
	int x;
	int y;
	int value;
	int counter;
	struct Node* prev;
} Node;

/* Stack implentation */
Node* new_node(int x, int y, int value, int counter);
void change_value(Node* node, int value);
int is_empty(Node* top);
void push(Node** top, int x, int y, int value, int counter);
int pop(Node** top, Board* b);
int top(Node* top);

/* input checking functions */
#define NOT_INT 1
#define NOT_FLOAT 1
int check_if_int(char* s, int* flag);
float check_if_float(char* s, int* flag);
int* set_params_int(Command* cmd, int* flags);

/* extra board functions */
void printBoard(Board* b);
int check_cell_errorness(Board* b, int x, int y, int val);
int out_of_range(Board* b, int* args);
void board_errorness(Board* b);

/* game saving functions */
#define FILE_FORMAT_ERROR 1
#define FILE_NOT_FOUND 2
#define FILE_READ 3
#define FILE_UNSOLVABLE 4
#define FILE_SAVED 5
int save_board(Board* b, char file_path[]);
int read_file(Board** old_b, char file_path[], int mode);

#endif /* end of include guard: UTIL_H_ */
