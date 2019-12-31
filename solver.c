#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "solver.h"
#include "printing.h"
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
	if (validate(i, j, b)) { /*VALID -continue to the next cell*/
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
    return 0;
}

/*Create a legal set of numbers for cell [i][j]*/
/*not sure about the allocation here,
* maybe it's better to allocate the array outside the function and send it as a parameter(?)
* need to free the memory(?)
*/
int* legalNumbers(int i, int j, Board* b) {
	int* legalArr = calloc(b->size , sizeof(int));
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
	for (index = 0; index < b->size; index++) {
		printf("this is a legal number: %d\n", legalArr[index]);
	}
	return legalArr;
}

int getNotZero(int* arr, int arr_size) {
	printf("hello from the getNotZero FUNCTION\n");
	int i;
	int len = 0;
	/* calculate the number of integers different to 0 */
	for (i = 0; i < arr_size; i++) {
		if (arr[i] != 0) {
			len++;
		}
	}

	return len;
}

/*Create a solution for an empty board with randomize backtracking*/
/*PROBLEM: Randomize also for 1 legal number*/
int createSolution2(int i, int j, Board* b) {
	int t;
	int index, randIndex, tempNum;
	int* legalNums;
	int len;
	/*Done*/
	if (i == b->size) {
		printf("Solution is ready!\n");
		return 1;
	}
	legalNums = legalNumbers(i, j, b);
	len = b->size;
	printf("board after setting cell %d %d:\n", i, j);
	printBoard2(b);
	printf("legal number of cell %d %d:\n", i, j);
	for (t = 0; t < len; t++) {
		printf("%d ", legalNums[t]);
	}
	
	for (index = 0; index < len; index++) {
		int notZeroNums = getNotZero(legalNums, len);
		printf("not zero numbers: %d:\n", notZeroNums);
		if (notZeroNums == 0) {
			break;
		}
		else {
			printf("before choose_random from loop");
			tempNum = choose_random(legalNums, len);
		}
		
		printf("this is tempNum: %d", tempNum);
		b->solution[i][j] = tempNum;
		if (j == b->size - 1) {
			if (createSolution2(i + 1, 0, b) == 1) {
				return 1;
			}
		}
		else {
			if (createSolution2(i, j + 1, b) == 1) {
				return 1;
			}
		}
	}

	b->solution[i][j] = 0;
	return 0;
}

/* gets the ith random element */
int get_i_element(int* arr, int arr_size, int i) {
	int c = 0;
	int j;
	for (j = 0; j < arr_size; j++) {
		if (arr[j] != 0) {
			if (i == c) {
				c = arr[j];
				arr[j] = 0;
				return c;
			}
			c++;
		}
	}
	return 0;
}


        

/* given array returns a random number and set its value to 0 */
int choose_random(int* arr, int arr_size){
	int randNum;
    int len = 0;
    int r;
    /* calculate the number of integers different to 0 */
	len = getNotZero(arr, arr_size);
    if(len == 1){
        return get_i_element(arr, arr_size, 0);
    }
    r = rand() % len;
	randNum = get_i_element(arr, arr_size, r);
    return randNum;
}

int createSolution(int i, int j, Board* b) {
    /* TODO add - if (i == b->size) { - should be checked if correct */
	int randNum;
    int index;
    int* legalNums;
    legalNums = legalNumbers(i, j, b);
	for (index = 0; index < b->size; index++) {

        /* only use choose_random function to get next number */
		randNum = choose_random(legalNums, b->size);
        /* TODO find out where to deallocate - i think it should be before the return */
		b->solution[i][j] = randNum;
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
	printf("hello from the createSolution FUNCTION\n cell NOT Good!!\n");
    return -500;
}

