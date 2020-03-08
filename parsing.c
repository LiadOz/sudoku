#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parsing.h"
#include "board.h"


#define COMMAND_DELIMITER " \t\r\n"
#define COMMANDS_NUM 16
#define SUCCSESS 5

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

int param_in_range(Command* cmd){
    /* TODO */
    return -1;
}

int solve_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int edit_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int mark_errors_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int print_board_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int set_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int validate_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int guess_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int generate_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int undo_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int redo_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int save_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int hint_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int guess_hint_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int num_solutions_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int autofill_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int reset_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
int exit_command(Board* b, Command* cmd){
    /* TODO */
    return -1;
}
typedef struct {
    char* name;
    int optional_arg;
    int (*func)(Board* b, Command* cmd);
    int args_num;
    int available_in_solve;
    int available_in_edit;
    int available_in_init;
} User_Command;

User_Command commands[] = {
    {"solve",           0, solve_command,           0, 1, 1, 1},
    {"edit",            1, edit_command,            1, 1, 1, 1},
    {"mark_errors",     0, mark_errors_command,     1, 1, 0, 0},
    {"print_board",     0, print_board_command,     0, 1, 1, 0},
    {"set",             0, set_command,             3, 1, 1, 0},
    {"validate",        0, validate_command,        0, 1, 1, 0},
    {"guess",           0, guess_command,           1, 1, 0, 0},
    {"generate",        0, generate_command,        2, 0, 1, 0},
    {"undo",            0, undo_command,            0, 1, 1, 0},
    {"redo",            0, redo_command,            0, 1, 1, 0},
    {"save",            0, save_command,            1, 1, 1, 0},
    {"hint",            0, hint_command,            2, 1, 0, 0},
    {"guess_hint",      0, guess_hint_command,      2, 1, 0, 0},
    {"num_solutions",   0, num_solutions_command,   0, 1, 1, 0},
    {"autofill",        0, autofill_command,        0, 1, 0, 0},
    {"reset",           0, reset_command,           0, 1, 1, 0},
    {"exit",            0, exit_command,            0, 1, 1, 1}
};

int available_command(User_Command* uc, int mode){
    switch (mode) {
        case INIT:
            if(uc->available_in_init == 1)
                return SUCCSESS;
            break;
        case EDIT:
            if(uc->available_in_edit == 1)
                return SUCCSESS;
            break;
        case SOLVE:
            if(uc->available_in_solve == 1)
                return SUCCSESS;
            break;
    }
    return COMMAND_FAILED;
}

void print_available_modes(User_Command* uc){
    int add_delim = 0;
    if(uc->available_in_init){
        add_delim = 1;
        printf("init");
    }
    if(add_delim)
        printf(", ");
    if(uc->available_in_solve){
        add_delim = 1;
        printf("solve");
    }
    if(add_delim)
        printf(", ");
    if(uc->available_in_edit){
        add_delim = 1;
        printf("edit");
    }
    printf(".\n");
}

int check_args_num(User_Command* uc, int args){
        if((!uc->optional_arg && args != uc->args_num) || (uc->optional_arg && args > uc->args_num)){
            return COMMAND_FAILED;
        }
    return SUCCSESS;
}

int execute_command(Board* b, Command* cmd){
    int i;
    User_Command uc;
    int command_found = 0;
    for(i = 0; i < COMMANDS_NUM; i++){
        uc = commands[i];
        if(strcmp(cmd->name, uc.name)){
            command_found = 1;
            if(available_command(&uc, b->mode) == COMMAND_FAILED){
                printf("Error: %s not available in current mode it is available in ", uc.name);
                print_available_modes(&uc);
                return COMMAND_FAILED;
            }
            if (check_args_num(&uc, cmd->arg_length) == COMMAND_FAILED){
                printf("Error: %s takes %d arguments\n", uc.name, uc.args_num);
                return COMMAND_FAILED;
            }
            /* stages 3-5 of checking are handled in the following funcion */
            return uc.func(b, cmd);
        }
    }
    if (command_found == 0){
        printf("Error: invalid command\n");
        return COMMAND_FAILED;
    }
    return -1;
}
