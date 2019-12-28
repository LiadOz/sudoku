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
