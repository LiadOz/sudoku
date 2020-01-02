#include <stdio.h>
#include "board.h"
#include "printing.h"
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
			else if (b->state[x][y] != 0) {
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
/*BOARD SOLUTION PRINTER BY POINTER*/
void printSolution(Board* b) {
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
			else if (b->solution[x][y] != 0) {
				printf(" %d", b->solution[x][y]);
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
