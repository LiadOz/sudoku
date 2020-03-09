#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parsing.h"
#include "printing.h"
#include "board.h"
#include "lp_solver.h"
#include "util.h"

#define COMMAND_DELIMITER " \t\r\n"
#define COMMANDS_NUM 17
#define SUCCSESS 5
#define UNUSED(x) (void)(x)

#define ERRORNOUS_PRINT "Error: %s can't execute when board is errornous\n"
#define PARAMETER_ERROR "Error: %s argument exptected %s\n"

/* gets array input and args array and fills it with the number of arguemnts needed
 * returns the number of assigned arguemnts
 * according to the excuetable anything other than a number seems to be 0
 * when parsing each number gets reduced by 1
 */
int get_args(char* input, char args[MAX_ARGS_SIZE][MAX_INPUT_SIZE-1]){
    int i = 0;
    char* token;
    token = strtok(input, COMMAND_DELIMITER);
    token = strtok(NULL, COMMAND_DELIMITER);
    while(token != NULL){
        strcpy(args[i++], token);
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

int solve_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int edit_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int mark_errors_command(Board* b, Command* cmd){
    int flag;
    int arg = check_if_int(cmd->args[0], &flag);
    if(flag == NOT_INT){
        printf(PARAMETER_ERROR, "1st", "int");
        return COMMAND_FAILED;
    }
    if (arg == NO_MARK_ERRORS){
        b->mark_errors = NO_MARK_ERRORS;
    }
    else if (arg == MARK_ERRORS) {
        b->mark_errors = MARK_ERRORS; 
    }
    else {
        printf("Error: first agument should be either %d or %d\n", NO_MARK_ERRORS, MARK_ERRORS);
        return COMMAND_FAILED;
    }
    return SUCCSESS;
}
int print_board_command(Board* b, Command* cmd){
    printBoard(b);
    UNUSED(cmd);
    return SUCCSESS;
}
int set_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int validate_command(Board* b, Command* cmd){
    if(b->wrong_cells){
        printf(ERRORNOUS_PRINT, cmd->name);
        return COMMAND_FAILED;
    }
    if(validate_board(b) == SOLUTION_FOUND)
        printf("The board is solvable\n");
    else
        printf("The board is unsolvable\n");
    return SUCCSESS;
}
int guess_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int generate_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int undo_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int redo_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int save_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int hint_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int guess_hint_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int num_solutions_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int autofill_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int reset_command(Board* b, Command* cmd){
    UNUSED(b);
    UNUSED(cmd);
    /* TODO */
    return -1;
}
int exit_command(Board* b, Command* cmd){
    printf("Exiting...\n");
    free_board(b);
    exit(0);
    UNUSED(cmd);
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

int execute_command_temp(Board* b, Command* cmd){
    int i;
    User_Command uc;
    int command_found = 0;
    b->mode = SOLVE;
    for(i = 0; i < COMMANDS_NUM; i++){
        uc = commands[i];
        if(!strcmp(cmd->name, uc.name)){
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
