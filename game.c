#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "parsing.h"
#include "game.h"
#include "printing.h"
#include "solver.h"

void exit_game(Board *b){
    printf("Exiting...\n");
    free_board(b);
    exit(0);
}
/* executes commands to the board */
int execute_command(Board *b, Command *cmd){
    char* operation = cmd->name;
    int* args = cmd->args;
    int length = cmd->arg_length;
    if(operation == NULL){
        return 0;
    }
    if(strcmp(operation, SET_CELL_COMMAND) == 0){
        if(length < SET_READ_ARGS){
            printf(INVALID_COMMAND);
            return 0;
        }
        if(set_cell(b, args[1]-1, args[0]-1, args[2])){
            return PRINT_AFTER;
        }
        return 0;
    }
    else if(strcmp(operation, HINT_CELL_COMMAND) == 0){
        if(length < HINT_READ_ARGS){
            printf(INVALID_COMMAND);
            return 0;
        }
        hint(b, args[1]-1, args[0]-1);

    }
    /*
    else if(strcmp(operation, VALIDATE_COMMAND) == 0){
        validate();
    }
    else if(strcmp(operation, RESTART_COMMAND) == 0){
        restart();
        return PRINT_AFTER;
    }
    */
    else if(strcmp(operation, EXIT_COMMAND) == 0){
        exit_game(b);
    }
    else{
        printf(INVALID_COMMAND);
        return 0;
    }
    return 1;
}

int get_cells_to_keep_from_user(Board *b){
    int input;
    printf("Please enter the number of cells to fill [0-%d]:\n", b->size*b->size-1);
    while(1){
        scanf("%d", &input);
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

int game_flow(){
    /* variable and board init */
    Board b;
    Command cmd;
    init_board(&b, 3, 3);
    createSolution(0, 0, &b);
    set_from_solution(&b, get_cells_to_keep_from_user(&b));
    /* get number of cells from the user and make the state */
    /*cells = 2;*/
    /*create_board_state(&b, cells);*/
	printBoard(&b);

    /* the game loop starts until the user uses the exit command or finished the board */
    while(1){
        do{
        user_input(&cmd);
        } while(execute_command(&b, &cmd) != PRINT_AFTER);
        printBoard(&b);

        /* if the board is finished then only restart and exit should be allowed */
    }
    return 1;
}
