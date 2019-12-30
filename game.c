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
        set_cell(b, args[1]-1, args[0]-1, args[2]);
        return PRINT_AFTER;
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

int test_game(){
    Board b2;
    Command cmd;
    me();
    init_board(&b2, 3, 3);
    createSolution(0, 0, &b2);
	printBoard2(&b2);
    while(1){
        do{
        user_input(&cmd);
        } while(execute_command(&b2, &cmd) != PRINT_AFTER);
        printBoard(&b2);
    }
    free_board(&b2);
    return 1;
}
