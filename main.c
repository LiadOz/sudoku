#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "cli.h"

#define WELCOME_MESSAGE "****    SUDOKU GAME    ****\n"

int main(){
    Board* b = NULL;
    printf(WELCOME_MESSAGE);

    while(1){
        execute_command(&b);
        /* test check if the board is finished */
		if (b->mode != EDIT) {
			if (b != NULL && (b->filled_cells == (b->size * b->size))) {
				if (b->wrong_cells == 0) {
					printf("The puzzle was solved successfully!\n");
					free(b);
					b = NULL;
				}
				else {
					printf("Board is errorness, keep trying..\n");
				}
			}
		}
    }
    return 1;
}
