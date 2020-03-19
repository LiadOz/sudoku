#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "parsing.h"
#include "game.h"
#include "util.h"
#include "moves.h"
#include "backtrack.h"

/* the entire game loop mechanic */
int game_flow(){
    /* variable and board init */
    /*
    Board b = init_game(3, 3);
    */
    Board* b = NULL;
    Command cmd;
    printf("****    SUDOKU GAME    ****\n");
    /*
    int next_command;
    int game_finished = 0; 
    */
    /* if the board is finished then only restart and exit should be allowed */

    /* the game loop starts until the user uses the exit command or finished the board */
    while(1){
        printf("Enter next command:\n");
        user_input(&cmd);
        execute_command(&b, &cmd);
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
        /*
        if (feof(stdin)){
            exit_game(&b);
        }
        next_command = execute_command(&b, &cmd, game_finished);
        if(next_command == PRINT_AFTER){
            printBoard(&b);
            if(b.filled_cells == b.size * b.size){
                printf("Puzzle solved successfully\n");
                game_finished = FINISHED;
            }
        }
        if(next_command == RESTART_AFTER){
            free_board(&b);
            b = init_game(3, 3);
            game_finished = 0;
        }
        */
    }
    return 1;
}

/*****************************************************************
 * ******************* HW3 Code *********************************
 ****************************************************************/

/* exits the game and frees allocated memory */
/*
void exit_game(Board *b){
    printf("Exiting...\n");
    free_board(b);
    exit(0);
} */
/* executes commands to the board */
/*
int execute_command(Board *b, Command *cmd,int game_finished){
	Move* curr_move = NULL;
	char* operation = cmd->name;
    int* args = cmd->args;
    int length = cmd->arg_length;
    if(operation == NULL){
        return 0;
    }
    if(strcmp(operation, SET_CELL_COMMAND) == 0 && game_finished != FINISHED){
        if(length < SET_READ_ARGS){
            printf(INVALID_COMMAND);
            return 0;
        }
        if(set_cell(b, args[1]-1, args[0]-1, args[2], &curr_move)){
			if (curr_move) {
				add_moves_to_board(b, curr_move);
			}
            return PRINT_AFTER;
        }
        return 0;
    }
    else if(strcmp(operation, HINT_CELL_COMMAND) == 0 && game_finished != FINISHED){
        if(length < HINT_READ_ARGS){
            printf(INVALID_COMMAND);
            return 0;
        }
        hint(b, args[1]-1, args[0]-1);

    }
    else if(strcmp(operation, VALIDATE_COMMAND) == 0 && game_finished != FINISHED){
        solveByDBT(0, 0, b);
    }
    else if(strcmp(operation, RESTART_COMMAND) == 0){
        return RESTART_AFTER;
    }
	else if (strcmp(operation, RESET_COMMAND) == 0) {
		reset(b);
		return PRINT_AFTER;
	}
	else if(strcmp(operation, UNDO_COMMAND) == 0) {
		if (undo(b, 0)) {
			return PRINT_AFTER;
		}
		printf(INVALID_COMMAND);
		return 0;
	}
	else if (strcmp(operation, REDO_COMMAND) == 0) {
		if (redo(b, 0)) {
			return PRINT_AFTER;
		}
		printf(INVALID_COMMAND);
		return 0;
	}
	else if (strcmp(operation, NUM_SOLUTIONS_COMMAND) == 0) {
		printf("There are %d possible solutions for this board\n", num_of_solutions(b));
		return 0;
	}
	else if (strcmp(operation, AUTOFILL_COMMAND) == 0) {
		if (autofill(b, &curr_move)) {
			add_moves_to_board(b, curr_move);
		}
		return PRINT_AFTER;
	}
    else if(strcmp(operation, EXIT_COMMAND) == 0){
        exit_game(b);
    }
    else{
        printf(INVALID_COMMAND);
        return 0;
    }
    return 1;
}
*/

/* gets the number of cells to keep in solution */
/*
int get_cells_to_keep_from_user(Board *b){
    int input;
    printf("Please enter the number of cells to fill [0-%d]:\n", b->size*b->size-1);
    while(1){
        scanf("%d\n", &input);
        if(feof(stdin)){
            exit_game(b);
        }
        if(input >= 0 && input <= b->size*b->size-1) {
            break;
        }
        printf("Error: invalid number of cells to fill (should be between 0 and %d)\n", b->size*b->size-1);
    }
    return input;
}
*/

/* initializing the board and the entire game according to board size */
/*
Board init_game(int width, int height){
    Board b;
    init_board(&b, width, height);
    createSolution(0, 0, &b);
    set_from_solution(&b, get_cells_to_keep_from_user(&b));
    set_from_solution(&b, 20);
	printBoard(&b);

    return b;
}*/
