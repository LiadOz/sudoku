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

/*copy the solved state into solution*/
void copySolution(Board* b) {
	int i, j;
	for (i = 0; i < b->size; i++) {
		for (j = 0; j < b->size; j++) {
			b->solution[i][j] = b->state[i][j];
		}
	}
}

/*BACKTRACKING SOLVING BOARD*/
int solveByDBT(int i, int j, Board* b) {
	int index;
	/*FOUND A SOLUTION*/
	if (i == b->size) {
		copySolution(b);
		printf("Validation passed: board is solvable\n");
		return 1;
	}
	
	if (b->state[i][j] != 0) {
		if (j == b->size - 1) {
			if (solveByDBT(i + 1, 0, b) == 0) {
				if (i == 0 && j == 0) {
					printf("Validation failed: board is unsolvable\n");
				}
				return 0;
			}
			return 1;
		}
		else {
			if (solveByDBT(i, j + 1, b) == 0) {
				if (i == 0 && j == 0) {
					printf("Validation failed: board is unsolvable\n");
				}
				return 0;
			}
			return 1;
		}
	}

	for (index = 1; index < b->size + 1; index++) {
		if (valid_set_value(b, i, j, index)) {
			b->state[i][j] = index;
			/*go to next cell*/
			if (j == b->size - 1) {
				if (solveByDBT(i + 1, 0, b) == 1) {
					b->state[i][j] = 0;
					return 1;
				}
			}
			else {
				if (solveByDBT(i, j + 1, b) == 1) {
					b->state[i][j] = 0;
					return 1;
				}
			}
		}
	}
	b->state[i][j] = 0;
	if (i == 0 && j == 0) {
		printf("Validation failed: board is unsolvable\n");
	}
	return 0;
}


/*Create a legal set of numbers for cell [i][j]*/
/*not sure about the allocation here,
* maybe it's better to allocate the array outside the function and send it as a parameter(?)
* need to free the memory(?)
*/
int* legalNumbers(int i, int j, Board* b, int* legalArr) {
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

int getNotZero(int* arr, int arr_size) {
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

/*Create a solution for an empty board with randomize backtracking*/
int createSolution(int i, int j, Board* b) {
	int index, tempNum;
	int* legalNums = calloc(b->size, sizeof(int));
	int len;
    if(legalNums == NULL){
        printf("Error: calloc has failed\n");
        exit(0);
    }
	if (i == b->size) {
        free(legalNums);
		return 1;
	}
	legalNums = legalNumbers(i, j, b, legalNums);
	len = b->size;
	for (index = 0; index < len; index++) {
		int notZeroNums = getNotZero(legalNums, len);
		if (notZeroNums == 0) {
			break;
		}
		else {
			tempNum = choose_random(legalNums, len);
		}
		
		b->solution[i][j] = tempNum;
		if (j == b->size - 1) {
			if (createSolution(i + 1, 0, b) == 1) {
                free(legalNums);
				return 1;
			}
		}
		else {
			if (createSolution(i, j + 1, b) == 1) {
                free(legalNums);
				return 1;
			}
		}
	}

	b->solution[i][j] = 0;
    free(legalNums);
	return 0;
}
