#ifndef PARSING_H
#define PARSING_H

#include "board.h"
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS_SIZE 3
#define COMMAND_EXECUTED 1
#define COMMAND_FAILED 0
#define PARAMETER_ERROR "Error: %s argument exptected %s\n"

/* name - name of the function to execute
 * args - the arguments
 * arg_length - length of the arguments
 * eof - determines if eof has been reached */
typedef struct {
    char* name;
    char args[MAX_ARGS_SIZE][MAX_INPUT_SIZE-1];
    int arg_length;
} Command;

void user_input();
int execute_command(Board** board_pointer, Command* cmd);

#endif
