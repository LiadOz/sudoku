#ifndef CLI_H
#define CLI_H

#include "board.h"
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS_SIZE 3
#define COMMAND_EXECUTED 1
#define COMMAND_FAILED 0
#define TOO_MUCH_CHARS 1
#define PARAMETER_ERROR "Error: %s argument exptected %s\n"

/* name - name of the function to execute
 * args - the arguments
 * arg_length - length of the arguments
 * c_status - tells status of command (if it has too much chars...) */
typedef struct {
    char* name;
    char args[MAX_ARGS_SIZE][MAX_INPUT_SIZE-1];
    int arg_length;
    int c_status;
} Command;

int execute_command(Board** board_pointer);

#endif
