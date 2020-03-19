/*
 * This module serves the as the command line interface between the user and the program
 *
 * Its only function execute_command takes a double pointer to board and asks the user
 * for a new command it will then check for error in the user input and if the command
 * is valid it will be executed from the other modules.
 */
#ifndef CLI_H
#define CLI_H

#include "board.h"
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS_SIZE 3
#define COMMAND_EXECUTED 1
#define COMMAND_FAILED 0
#define TOO_MUCH_CHARS 1
#define PARAMETER_ERROR "Error: %s argument exptected %s\n"

/*
 * A structure that holds a user inputted command
 * name - name of the function to execute
 * args - the arguments
 * arg_length - length of the arguments
 * c_status - tells status of command (if it has too much chars...) 
 */
typedef struct {
    char* name;
    char args[MAX_ARGS_SIZE][MAX_INPUT_SIZE-1];
    int arg_length;
    int c_status;
} Command;

int execute_command(Board** board_pointer);

#endif
