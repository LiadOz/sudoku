#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parsing.h"

#define COMMAND_DELIMITER " \t\r\n"

/*
static void set(int x, int y, int z){
    printf("setting cell (%d,%d) to %d (placeholder)\n", x, y, z);
}
static void hint(int x, int y){
    printf("hint for cell (%d,%d) is 0 (placeholder)\n", x, y);
}
static void validate(){
    printf("validating ... (placeholder)\n");
}
static void restart(){
    printf("restarting... (placeholder)\n");
}
static void exit_game(){
    printf("exiting... (placeholder)\n");
}
*/

/* gets array input and args array and fills it with the number of arguemnts needed
 * returns the number of assigned arguemnts
 * according to the excuetable anything other than a number seems to be 0
 * when parsing each number gets reduced by 1
 */
static int get_args(char* input, int* args){
    int i = 0;
    int arg;
    char* token;
    token = strtok(input, COMMAND_DELIMITER);
    token = strtok(NULL, COMMAND_DELIMITER);
    while(token != NULL){
        arg = atoi(token);
        args[i++] = arg;
        token = strtok(NULL, COMMAND_DELIMITER);
    }
    return i;
}

/* sets a Command struct from user input */
void user_input(Command *cmd){
    char input[MAX_INPUT_SIZE];
    char input_copy[MAX_INPUT_SIZE];
    fgets(input, MAX_INPUT_SIZE, stdin);  
    strcpy(input_copy, input);
    cmd->name = strtok(input_copy, COMMAND_DELIMITER);
    cmd->arg_length = get_args(input, cmd->args);
}
/* takes care of user input after the game has been initialized */
void user_input_old(){
    /* takes input from the user then according to the command scans for the arguments */
    /*
    char input[MAX_INPUT_SIZE];
    char input_copy[MAX_INPUT_SIZE];
    char* operation;
    fgets(input, MAX_INPUT_SIZE, stdin);  
    strcpy(input_copy, input);
    operation = strtok(input_copy, COMMAND_DELIMITER);
    if(strcmp(operation, SET_CELL_COMMAND) == 0){
        int args[SET_READ_ARGS];
        if(get_args(input, args, SET_READ_ARGS)){
            set(args[0], args[1], args[2]);
        }
    }
    else if(strcmp(operation, HINT_CELL_COMMAND) == 0){
        int args[HINT_READ_ARGS];
        if(get_args(input, args, HINT_READ_ARGS)){
            hint(args[0], args[1]);
        }
    }
    else if(strcmp(operation, VALIDATE_COMMAND) == 0){
        validate();
    }
    else if(strcmp(operation, RESTART_COMMAND) == 0){
        restart();
    }
    else if(strcmp(operation, EXIT_COMMAND) == 0){
        exit_game();
    }
    else{
        printf(INVALID_COMMAND);
    }
    */
}
