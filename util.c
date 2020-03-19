#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "cli.h"
#include "board.h"

#define READ_MAX_SIZE 3
/*generic line length calculation*/
int lineCalc(Board* b) {
	return 4 * b->size + b->height + 1;
}

/*prints the board to the screen*/
void printBoard(Board* b) {
	int lineLength = lineCalc(b);
	int x, y, i;

	for (x = 0; x < b->size; x++) {
		if (x % b->height == 0) {
			for (i = 0; i < lineLength; i++) {
				printf("-");
			}
			printf("\n");
		}
		for (y = 0; y < b->size; y++) {
			if (y % b->width == 0) {
				printf("|");
			}
			/* fixed cell */
			if ((!(b->mode == EDIT) && b->fixed[x][y] == 1) && b->state[x][y] != 0) {
				printf(" ");
				printf("%2d.", b->state[x][y]);
			}
			/* errorness cell */
			else if ((b->mark_errors == MARK_ERRORS || b->mode == EDIT) && b->wrong[x][y] == 1 && b->state[x][y] != 0) {
				printf(" ");
				printf("%2d*", b->state[x][y]);
			}
			/* regular cell */
			else if (b->state[x][y] != 0) {
				printf(" ");
				printf("%2d", b->state[x][y]);
				printf(" ");
			}
			/* empty cell */
			else {
				printf(" ");
				printf("   ");
			}
		}
		printf("|");
		if (x == b->size - 1) {
			printf("\n");
			for (i = 0; i < lineLength; i++) {
				printf("-");
			}
		}
		printf("\n");
	}
}

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
	int i;
	int* args = (int*)malloc(cmd->arg_length * sizeof(int));
	if (args == NULL) {
		printf("Error: malloc has failed\n");
		exit(0);
	}
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


Node* new_node(int x, int y, int value, int counter) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		printf("Error: malloc has failed\n");
		exit(0);
	}
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

void push(Node** top, int x, int y, int value, int counter) {
	Node* node = new_node(x, y, value, counter);
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

int save_board(Board* b, char file_path[]){
    FILE *fptr;
    int i, j;
    fptr = fopen(file_path, "w");
    if(fptr == NULL){
        return FILE_NOT_FOUND;
    }
    /* prints height and width */
    fprintf(fptr, "%d %d\n", b->height, b->width);

    /* prints cells */
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            if(j != 0)
                fprintf(fptr, " ");
            fprintf(fptr, "%d", b->state[i][j]);
            if(b->fixed[i][j])
                fprintf(fptr, ".");
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
    return FILE_SAVED;
}

/* Reads a file and creates a new board according to it */
int read_file(Board** old_b, char file_path[], int mode){
	FILE* fptr;
	unsigned long pos;
	int height, width;
	int i, j, flag = 0;
	char temp[READ_MAX_SIZE];
	char* point_ptr;
	int val;
    Board* b;

    fptr = fopen(file_path, "r");
    if(fptr == NULL){
        return FILE_NOT_FOUND;
    }
    b = (Board *)malloc(sizeof(Board));
	if (b == NULL) {
		printf("Error: malloc has failed\n");
		exit(0);
	}
    fscanf(fptr, "%d", &height);
    fscanf(fptr, "%d", &width);
    if(height < 1 || width < 1)
        return FILE_FORMAT_ERROR;

    init_board(b, width, height);
    b->mode = mode;
    /* first insert the fixed values and check whether the board is solvable */
    pos = ftell(fptr);
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            /* testing if inputs exists */
            if(fscanf(fptr, "%s", temp) == EOF)
                return FILE_FORMAT_ERROR;
            point_ptr = strchr(temp, '.');
            /* inserting fixed values */
            if(point_ptr != NULL){
                *point_ptr = '\0';
                val = atoi(temp);
                if(val > b->size)
                    return FILE_FORMAT_ERROR;
                else if(!valid_set_value(b, i, j, val))
                    return FILE_UNSOLVABLE;
                set_cell(b, i, j, val);
                b->fixed[i][j] = 1;
            }
        }
    }
    fseek(fptr, pos, 0);
    /* then insert the rest of the values */
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            fscanf(fptr, "%s", temp);
            point_ptr = strchr(temp, '.');
            if(point_ptr == NULL){
                val = check_if_int(temp, &flag);
                if(flag == NOT_INT)
                    return FILE_FORMAT_ERROR;
                val = atoi(temp);
                if(val > b->size)
                    return FILE_FORMAT_ERROR;
                set_cell(b, i, j, val);
            }
        }
    }
    fclose(fptr);
    /* replace the old board */
    if(*old_b){
        free_board(*old_b);
    }
    *old_b = b;
    return FILE_READ;
}
