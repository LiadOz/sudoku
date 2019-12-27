#include <stdlib.h>
#include <stdio.h>

/*
 * Board struct:
 * solution - the last solution given by the validate method
 * state - the state of the board with user guesses
 * fixed - determine wheter a cell is fixed (1) or not (0)
 */

typedef struct {
    int **solution;
    int **state;
    int **fixed;
    int width;
    int height;
    int size;
} Board;
void sample_board(Board* p);

/*
 * used to populate the board
 */
void init_board(Board* b, int width, int height){
    int i;
    b->solution = malloc(width * height * sizeof(int *));
    b->state = malloc(width * height * sizeof(int *));
    b->fixed = malloc(width * height * sizeof(int *));
    for(i = 0; i < width * height; i++){
        b->solution[i] = calloc(width * height, sizeof(int));
        b->state[i] = calloc(width * height, sizeof(int)); 
        b->fixed[i] = calloc(width * height, sizeof(int));
    }
    b->width = width;
    b->height = height;
    b->size = width * height;
    /* create random board */
}

/* used to free allocation when program exits */
void free_board(Board* b){
    int i;
    for(i = 0; i < b->size; i++){
        free(b->solution[i]);
        free(b->state[i]);
        free(b->fixed[i]);
    }
    free(b->solution);
    free(b->state);
    free(b->fixed);
}

/* checks if number exists in array if true return 1 */
int number_in_array(int* arr, int number, int size){
    int i;
    for(i = 0; i < size; i++){
        if(arr[i] == number){
            return 1;
        }
    }
    return 0;
}

/* Returns array of all the numbers in the row
 * assumes row is in the range
 * dynamically allocate memory must be freed
 */
int* get_row_numbers(Board* b, int row){
    int *arr = malloc(b->size * sizeof(int));
    int i;
    for(i = 0; i < b->size; i++){
        arr[i] = b->state[row][i];
    }
    return arr;
}


/* Returns array of all the numbers in the column 
 * assumes column is in the range
 * dynamically allocate memory must be freed
 */
int* get_column_numbers(Board* b, int col){
    int *arr = malloc(b->size * sizeof(int));
    int i;
    for(i = 0; i < b->size; i++){
        arr[i] = b->state[i][col];
    }
    return arr;
}

/* returns the block index of coordinates */
int get_coordinate_block_number(Board* b, int x, int y){
    return y / b->height * b->height + x / b->width;
}

/* Returns array of all the numbers in the block
 * blocks are defined from up to bottom left to right
 * the number of blocks is width * height
 * assumes block is in the range
 * dynamically allocate memory must be freed
 */
int* get_block_numbers(Board* b, int block_number){
    int *arr = malloc(b->size * sizeof(int));
    int block_y = block_number / b->height;
    int block_x = block_number - (block_y * b->height);
    int start_x = b->width * block_x;
    int start_y = b->height* block_y;
    int i, j, c = 0;
    for(i = start_y; i < b->height + start_y; i++){
        for(j = start_x; j < b->width + start_x; j++){
            arr[c++] = b->state[i][j];
        }
    }
    return arr;
}

/* checks if number is in the area if true return 1
 * mode 0 = row
 * mode 1 = column
 * mode 2 = block
 */ 
int number_in_area(Board* b, int mode, int index, int number){
    int* arr;
    int result; 
    if(mode == 0){
        arr = get_row_numbers(b, index);
    }
    else if(mode == 1){
        arr = get_column_numbers(b, index);
    }
    else if(mode == 2){
        arr = get_block_numbers(b, index);
    }
    else {
        return -1;
    }

    result = number_in_array(arr, number, b->size);
    free(arr);
    return result;
}

/* checks if value can be inserted into x y coordinates return 1 if true */
int valid_set_value(Board* b, int x, int y, int val){
    if(!number_in_area(b, 0, y, val) && !number_in_area(b, 1, x, val) && !number_in_area(b, 2, get_coordinate_block_number(b, x, y), val)){
        return 1;
            }
    return 0;
}

/* sets a cell to value with x, y coordinates
 * x and y starts at 1
 */
void set_cell(Board* b, int x, int y, int val){
    int i;
    if(b->fixed[x][y] == 1){
        printf("Error: cell is fixed\n");
        return;
    }
    if(valid_set_value(b, x, y, val)){
        b->state[x][y] = val;
    }
    else{
        printf("Error: value is invalid\n");
    }
}

/* gives hint to cell (x, y) when they start at 1 */
void hint(Board* b, int x, int y){
    printf("Hint: set cell to %d\n", b->solution[x][y]);
}

/*GENERIC LINE LENGTH CALCULATOR*/
int lineCalc(Board* b) {
	int nums = b->width * b->height;
	int pipes = b->height + 1;
	int spaces = nums + pipes - 1;
	int lineLength = spaces + pipes + 2*nums;

	return lineLength;
}

/*BOARD PRINTER BY POINTER*/
void printBoard(Board* b) {
	/*LINE LENGTH CALC*/
	int lineLength = lineCalc(b);

	/*DECLARAITIONS*/
	int x, y, i;

	/*ROW*/
	for (x = 0; x < b->size; x++) {
		/*PRINT LINE BREAK*/
		if (x % b->height == 0) {
			for (i = 0; i < lineLength; i++) {
				printf("-");
			}
			printf("\n");
		}
		/*Column*/
		for (y = 0; y < b->size; y++) {
			/*PIPE LOGIC*/
			if (y % b->width == 0) {
				printf("|");
				/*PRINT SPACE AFTER PIPE*/
				if (y < b->size - 1) {
					printf(" ");
				}
			}
			/*FIXED CELL*/
			if (b->fixed[x][y] == 1) {
				printf(".%d", b->solution[x][y]);
			}
			/*REGULAR CELL*/
			else if (&b->state[x][y] != 0) {
				printf(" %d", b->state[x][y]);
			}
			/*EMPTY CELL*/
			else {
				printf("  ");
			}
			/*PRINT SPACE AFTER DIGIT*/
			printf(" ");
		}
		printf("|");
		/*LAST LINE BREAK*/
		if (x == b->size - 1) {
			printf("\n");
			for (i = 0; i < lineLength; i++) {
				printf("-");
			}
		}
		printf("\n");
	}
}

/*VALIDATION*/

int validateRow(int i, int j, Board* b) {
	int value = b->solution[i][j];
	int index;
	for (index = 0; index < b->size; index++) {
		if (index != j) {
			if (b->solution[i][index] == value) {
				return 0;
			}
		}
	}
	
	return 1;
}

int validateColumn(int i, int j, Board* b) {
	int value = b->solution[i][j];
	int index;
	for (index = 0; index < b->size; index++) {
		if (index != i) {
			if (b->solution[index][j] == value) {
				return 0;
			}
		}
	}

	return 1;
}

int validateBlock(int i, int j, Board* b) {
	int x, y;
	int value = b->solution[i][j];
	for (x = i - (i % b->height); x < b->height + i - (i % b->height); x++) {
		for (y = j - (j % b->width); y < b->width + j - (j % b->width); y++) {
			if (x == i && y == j) {
				continue;
			}
			if (b->solution[x][y] == value) {
				return 0;
			}
		}
	}
	return 1;
}

int validate(int i, int j, Board* b) {
	if (!validateRow(i, j, b)) {
		return 0;
	}
	if (!validateColumn(i, j, b)) {
		return 0;
	}
	if (!validateBlock(i, j, b)) {
		return 0;
	}

	return 1;
}

/*BACKTRACKING SOLVING BOARD*/

int solveByDBT(int i, int j, Board* b) {
	/*DONE SOLVING*/
	if (i == b->size) {
		printf("SUDOKU SOLVED!");
		return 1;
	}
	/*No Solution - return 0*/
	if (b->solution[i][j] > b->size) {
		if (i == 0 && j == 0) {
			printf("there is no solution for this board");
			return 0;
		}
		/*Go Back - return */
		b->solution[i][j] = 0;
		if (j == 0) {
			solveByDBT(i - 1, b->size - 1, b);
		}
		else {
			solveByDBT(i, j - 1, b);
		}
		
	}
	/*FIXED CELL - move to the next cell*/
	if (b->fixed[i][j] == 1) {
		if (j == b->size - 1) {
			solveByDBT(i + 1, 0, b);
		}
		else {
			solveByDBT(i, j + 1, b);
		}
		
	}
	b->state[i][j] += 1;
	/*RECURSION STEP*/
	if (validte(i, j, b)) { /*VALID -continue to the next cell*/
		if (j == b->size - 1) {
			solveByDBT(i + 1, 0, b);
		}
		else {
			solveByDBT(i, j + 1, b);
		}
	}
	/*NOT VALID - Try again on the same cell*/
	else {
		solveByDBT(i, j, b);
	}
}


/* testing things */
int main(){
    Board b2;
    init_board(&b2, 2, 6);
    /* sample_board(&b2); */
	printBoard(&b2);
    set_cell(&b2, 5, 5 ,2);
	printBoard(&b2);
    hint(&b2, 5, 6);
    set_cell(&b2, 5, 6 ,5);
	printBoard(&b2);
    free_board(&b2);
    return 0;
}

/*
 * create simple board for dubgging
 * initial state should look like:
 * size 2, 3
 * 1 2 | 3 4 | 5 6
 * 3 4 | 5 6 | 1 2
 * 5 6 | 1 2 | 3 4
 * ---------------
 * 2 1 | 5 6 | 0 0
 * 4 3 | 2 1 | 0 0
 * 6 5 | 4 3 | 0 0
 *
 */
void sample_board(Board* p){
    p->solution[0][0] = 1;
    p->solution[0][1] = 2;
    p->solution[0][2] = 3;
    p->solution[0][3] = 4;
    p->solution[0][4] = 5;
    p->solution[0][5] = 6;
    p->solution[1][0] = 3;
    p->solution[1][1] = 4;
    p->solution[1][2] = 5;
    p->solution[1][3] = 6;
    p->solution[1][4] = 1;
    p->solution[1][5] = 2;
    p->solution[2][0] = 5;
    p->solution[2][1] = 6;
    p->solution[2][2] = 1;
    p->solution[2][3] = 2;
    p->solution[2][4] = 3;
    p->solution[2][5] = 4;
    /* second half */
    p->solution[3][0] = 2;
    p->solution[3][1] = 1;
    p->solution[3][2] = 5;
    p->solution[3][3] = 6;
    p->solution[3][4] = 4;
    p->solution[3][5] = 3;
    p->solution[4][0] = 4;
    p->solution[4][1] = 3;
    p->solution[4][2] = 2;
    p->solution[4][3] = 1;
    p->solution[4][4] = 6;
    p->solution[4][5] = 5;
    p->solution[5][0] = 6;
    p->solution[5][1] = 5;
    p->solution[5][2] = 4;
    p->solution[5][3] = 3;
    p->solution[5][4] = 2;
    p->solution[5][5] = 1;

    p->state[0][0] = 1;
    p->state[0][1] = 2;
    p->state[0][2] = 3;
    p->state[0][3] = 4;
    p->state[0][4] = 5;
    p->state[0][5] = 6;
    p->state[1][0] = 3;
    p->state[1][1] = 4;
    p->state[1][2] = 5;
    p->state[1][3] = 6;
    p->state[1][4] = 1;
    p->state[1][5] = 2;
    p->state[2][0] = 5;
    p->state[2][1] = 6;
    p->state[2][2] = 1;
    p->state[2][3] = 2;
    p->state[2][4] = 3;
    p->state[2][5] = 4;
    p->state[3][0] = 2;
    p->state[3][1] = 1;
    p->state[3][2] = 5;
    p->state[3][3] = 6;
    p->state[4][0] = 4;
    p->state[4][1] = 3;
    p->state[4][2] = 2;
    p->state[4][3] = 1;
    p->state[5][0] = 6;
    p->state[5][1] = 5;
    p->state[5][2] = 4;
    p->state[5][3] = 3;

    p->fixed[0][0] = 1;
    p->fixed[0][1] = 1;
    p->fixed[0][2] = 1;
    p->fixed[0][3] = 1;
    p->fixed[0][4] = 1;
    p->fixed[0][5] = 1;
    p->fixed[1][0] = 1;
    p->fixed[1][1] = 1;
    p->fixed[1][2] = 1;
    p->fixed[1][3] = 1;
    p->fixed[1][4] = 1;
    p->fixed[1][5] = 1;
    p->fixed[2][0] = 1;
    p->fixed[2][1] = 1;
    p->fixed[2][2] = 1;
    p->fixed[2][3] = 1;
    p->fixed[2][4] = 1;
    p->fixed[2][5] = 1;
    p->fixed[3][0] = 1;
    p->fixed[3][1] = 1;
    p->fixed[3][2] = 1;
    p->fixed[3][3] = 1;
    p->fixed[3][4] = 0;
    p->fixed[3][5] = 0;
    p->fixed[4][0] = 1;
    p->fixed[4][1] = 1;
    p->fixed[4][2] = 1;
    p->fixed[4][3] = 1;
    p->fixed[4][4] = 0;
    p->fixed[4][5] = 0;
    p->fixed[5][0] = 1;
    p->fixed[5][1] = 1;
    p->fixed[5][2] = 1;
    p->fixed[5][3] = 1;
    p->fixed[5][4] = 0;
    p->fixed[5][5] = 0;
}
