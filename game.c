#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "parsing.h"
#include "game.h"

/* executes commands to the board */
void execute_command(Board *b, Command *cmd){
    char* operation = cmd->name;
    int* args = cmd->args;
    int length = cmd->arg_length;
    if(strcmp(operation, SET_CELL_COMMAND) == 0){
        if(length < SET_READ_ARGS){
            printf(INVALID_COMMAND);
        }
        set_cell(b, args[0]-1, args[1]-1, args[2]);
    }
    else if(strcmp(operation, HINT_CELL_COMMAND) == 0){
        if(length < HINT_READ_ARGS){
            printf(INVALID_COMMAND);
        }
        hint(b, args[0]-1, args[1]-1);

    }
    /*
    else if(strcmp(operation, VALIDATE_COMMAND) == 0){
        validate();
    }
    else if(strcmp(operation, RESTART_COMMAND) == 0){
        restart();
    }
    else if(strcmp(operation, EXIT_COMMAND) == 0){
        exit_game();
    }
    */
    else{
        printf(INVALID_COMMAND);
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

/*Create a legal set of numbers for cell [i][j]*/
/*not sure about the allocation here,
* maybe it's better to allocate the array outside the function and send it as a parameter(?)
* need to free the memory(?)
*/
int* legalNumbers(int i, int j, Board* b) {
	int* legalArr = malloc(b->size * sizeof(int));
	int count = 0;
	int index;
	int valueInCell = b->solution[i][j];
	for (index = 1; index < b->size + 1; index++) {
		if (index != valueInCell) {
			b->solution[i][j] = index;
			if (validate(i, j, b)) {
				legalArr[count] = index;
				count++;
			}
		}
	}
	b->solution[i][j] = valueInCell;
	return legalArr;
}

/*Create a solution for an empty board with randomize backtracking*/
/*PROBLEM: Randomize also for 1 legal number*/
int createSolution(int i, int j, Board* b) {
	int index, randIndex, tempNum;
	/*Done*/
	if (i == b->size) {
		printf("Solution is ready!");
		return 1;
	}
	int* legalNums = legalNumbers(i, j, b);
	int len = sizeof(legalNums) / sizeof(int);
	for (index = 0; index < len; index++) {
		do {
			randIndex = rand() % len;
		} while (legalNums[randIndex] != 0);
		tempNum = legalNums[randIndex];
		legalNums[randIndex] = 0;
		b->solution[i][j] = tempNum;
		if (j == b->size - 1) {
			if (createSolution(i + 1, 0, b) == 1) {
				return 1;
			}
		}
		else {
			if (createSolution(i, j + 1, b) == 1) {
				return 1;
			}
		}
	}
	return 0;
}


int test_game(){
    Board b2;
    Command cmd;
    init_board(&b2, 2, 6);
	printBoard(&b2);
    while(1){
        user_input(&cmd);
        execute_command(&b2, &cmd);
        printBoard(&b2);
    }
    free_board(&b2);
    return 1;
}
