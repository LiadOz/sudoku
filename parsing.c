#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define SET_CELL_COMMAND "set"
#define HINT_CELL_COMMAND "hint"
#define VALIDATE_COMMAND "validate"
#define RESTART_COMMAND "restart"
#define EXIT_COMMAND "exit"
#define INVALID_COMMAND "Error: invalid command\n"
#define COMMAND_DELIMITER " \t\r\n"
#define SET_READ_ARGS 3
#define HINT_READ_ARGS 2

void set(int x, int y, int z){
    printf("setting cell (%d,%d) to %d (placeholder)\n", x, y, z);
}
void hint(int x, int y){
    printf("hint for cell (%d,%d) is 0 (placeholder)\n", x, y);
}
void validate(){
    printf("validating ... (placeholder)\n");
}
void restart(){
    printf("restarting... (placeholder)\n");
}
void exit_game(){
    printf("exiting... (placeholder)\n");
}

/* gets array input and args array and fills it with the number of arguemnts needed
 * returns 0 if failed and prints error message
 * according to the excuetable anything other than a number seems to be 0
 */
int get_args(char* input, int* args, int num){
    int i;
    int arg;
    char* token;
    strtok(input, COMMAND_DELIMITER);
    for(i = 0; i < num; i++){
        token = strtok(NULL, COMMAND_DELIMITER);
        if(token == NULL){
            printf(INVALID_COMMAND);
            return 0;
        }
        arg = atoi(token);
        args[i] = arg;
    }
    return 1;
}

/* takes care of user input after the game has been initialized */
int user_input(){
    /* takes input from the user then according to the command scans for the arguments */
    char input[MAX_INPUT_SIZE];
    char input_copy[MAX_INPUT_SIZE];
    char* operation;
    int i;
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
}
int main(){
    user_input();
    user_input();
    user_input();
}
