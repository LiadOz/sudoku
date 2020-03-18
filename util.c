#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "parsing.h"
#include "printing.h"

/* Returns NOT_INT if is not int
 * if it is int it returns the number */
int check_if_int(char* s, int* flag){
    int i_num = atoi(s);
	float f_num = atof(s);
	int f_as_i = f_num;
    if(i_num == 0 && s[0] != '0'){
        *flag = NOT_INT;
        return 0;
	}
	else if (f_as_i != f_num) {
		*flag = NOT_INT;
		return 0;
	}
    return i_num;
}

/* Returns NOT_FLOAT if is not float
 * if it is int it returns the number */
float check_if_float(char* s, int* flag){
    float val;
    if(sscanf(s, "%f", &val) != 0)
        return val;
    *flag = NOT_FLOAT;
    return 0;
}

int* set_params_int(Command* cmd, int* flags) {
	int* args = (int*)malloc(cmd->arg_length * sizeof(int));
	if (args == NULL) {
		printf("Error: malloc has failed\n");
		exit(0);
	}
	int i;
	for (i = 0; i < cmd->arg_length; i++) {
		args[i] = check_if_int(cmd->args[i], &(flags[i]));
		if (flags[i] == NOT_INT) {
			if (i == 0) {
				printf(PARAMETER_ERROR, "1st", "int");
			}
			else if (i == 1) {
				printf(PARAMETER_ERROR, "2nd", "int");
			}
			else {
				printf(PARAMETER_ERROR, "3rd", "int");
			}
		}
	}
	return args;
}

void errorness_row(Board* b, int x, int y, int val, int* error) {
	int i;
	for (i = 0; i < b->size; i++) {
		if (i == y) {
			continue;
		}
		if (b->state[x][i] == val && val != 0) {
			*error = 1;
			b->wrong[x][y] = 1;
			break;
		}
	}
}

void errorness_column(Board* b, int x, int y, int val, int* error) {
	int i;
	for (i = 0; i < b->size; i++) {
		if (i == x) {
			continue;
		}
		if (b->state[i][y] == val && val != 0) {
			*error = 1;
			b->wrong[x][y] = 1;
			break;
		}
	}
}

void errorness_block(Board* b, int x, int y, int val, int* error) {
	int i, j;

	for (i = (x - (x % b->height)); i < b->height + (x - (x % b->height)); i++) {
		for (j = (y - (y % b->width)); j < b->width + (y - (y % b->width)); j++) {
			if (i == x && j == y) {
				continue;
			}
			if (b->state[i][j] == val && val != 0) {
				*error = 1;
				b->wrong[x][y] = 1;
				break;
			}
		}
	}
}

/*call this after set cell's value (board->state)*/

int check_cell_errorness(Board* b, int x, int y, int val) {
	int error = 0;
	errorness_row(b, x, y, val, &error);
	errorness_column(b, x, y, val, &error);
	errorness_block(b, x, y, val, &error);
	return error;
}

void board_errorness(Board* b) {
	int i, j;
	b->wrong_cells = 0;
	for (i = 0; i < b->size; i++) {
		for (j = 0; j < b->size; j++) {
			if (check_cell_errorness(b, i, j, b->state[i][j])) {
				b->wrong_cells++;
			}
			else {
				b->wrong[i][j] = 0;
			}
		}
	}
}

int out_of_range(Board* b, int* args) {
	if ((args[0] - 1 > b->size) || ((args[1] - 1) > b->size) || (args[2] > b->size) ||
		(args[0] - 1 < 0) || ((args[1] - 1) < 0) || (args[2] < 0)) {
		return 1;
	}
	return 0;
}
