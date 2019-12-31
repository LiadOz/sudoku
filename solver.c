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
	printf("%d", b->size);
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
int createSolution2(int i, int j, Board* b) {
    int t;
	int index, randIndex, tempNum;
    int* legalNums;
    int len;
	/*Done*/
	if (i == b->size) {
		printf("Solution is ready!");
		return 1;
	}
	legalNums = legalNumbers(i, j, b);

	len = b->size;
    printBoard2(b);
    for(t = 0; t < len; t++){
        printf("%d ", legalNums[t]);
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

/* gets the ith random element */
int get_i_element(int* arr, int arr_size, int i) {
	int c = 1;
	int j;
	for (j = 0; j < arr_size; j++) {
		if (arr[j] != 0) {
			if (i == c) {
				arr[j] = 0;
				return arr[j];
			}
			c++;
		}
	}
}
        

/* given array returns a random number and set its value to 0 */
int choose_random(int* arr, int arr_size){
    int i;
    int len = 0;
    int r;
    /* calculate the number of integers different to 0 */
    for(i = 0; i < arr_size; i++){
        if(arr[i] != 0){
            len++;
        }
    }
    if(len == 1){
        return get_i_element(arr, arr_size, 1);
    }
    r = rand() % len;
    return get_i_element(arr, arr_size, r);
}

