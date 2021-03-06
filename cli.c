#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli.h"
#include "board.h"
#include "lp_solver.h"
#include "util.h"
#include "moves.h"
#include "backtrack.h"

#define COMMAND_DELIMITER " \t\r\n"
#define COMMANDS_NUM 17
#define SUCCSESS 5
#define DEFAULT_BOARD_SIZE 3
#define OVERFLOW 256
#define UNUSED(x) (void)(x)

#define ARGS_OUT_OF_RANGE "Error: the parameters are out of range, please enter a parameter between 1 to %d\n"
#define ERRORNOUS_PRINT "Error: command %s can't execute when board is errornous\n"


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
    while(token != NULL && i < MAX_ARGS_SIZE){
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
    if(strlen(input) > OVERFLOW){
        cmd->c_status = TOO_MUCH_CHARS;
        /* flushing the rest of input */
        while(!strchr(input, '\n'))
            if(!fgets(input, MAX_INPUT_SIZE, stdin))
                    break;
        return;
    }
    else
        cmd->c_status = 0;
    strcpy(input_copy, input);
    cmd->name = strtok(input_copy, COMMAND_DELIMITER);
    cmd->arg_length = get_args(input, cmd->args);
}

/******************************************************************************
 * This function is used in order to handle the return values 
 * of the save and load functions and to print the correct Error messages.
 * 
 * Returns COMMAND_FAILED if the read or write was unsuccsessful otherwise
 * SUCCESSS is returned.
*****************************************************************************/
int get_file_status(int status){
    if(status == FILE_FORMAT_ERROR){
        printf("Error: file format is incorrect\n");
        return COMMAND_FAILED;
    }
    if(status == FILE_NOT_FOUND){
        printf("Error: file not found\n");
        return COMMAND_FAILED;
    }
    if(status == FILE_UNSOLVABLE){
        printf("Error: file is not solvable\n");
        return COMMAND_FAILED;
    }
    return SUCCSESS;
}

/* checks for errornous and prints appropriate message */
int errornous_check(Board* b, Command* cmd){
    if(b->wrong_cells){
        printf(ERRORNOUS_PRINT, cmd->name);
        return COMMAND_FAILED;
    }
    return SUCCESS;
}

/* checks for errornous and for parameters in range */
int check_coordinates(Board* b, Command* cmd){
    int flag = 0;
    int y, x = check_if_int(cmd->args[0], &flag) - 1;
    if(flag == NOT_INT){
        printf(PARAMETER_ERROR, "1st", "int");
        return COMMAND_FAILED;
    }
    y = check_if_int(cmd->args[1], &flag) - 1;
    if(flag == NOT_INT){
        printf(PARAMETER_ERROR, "2nd", "int");
        return COMMAND_FAILED;
    }
    if(x < 0 || x >= b->size){
        printf("Error: 1st parameter expected range 1 up to %d\n", b->size);
        return COMMAND_FAILED;
    }
    if(y < 0 || x >= b->size){
        printf("Error: 2nd parameter expected range 1 up to %d\n", b->size);
        return COMMAND_FAILED;
    }
    if(errornous_check(b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    if(b->fixed[x][y]){
        printf("Error: cell is fixed\n");
        return COMMAND_FAILED;
    }
    if(b->state[x][y]){
        printf("Error: cell is already set\n");
        return COMMAND_FAILED;
    }
    return SUCCESS;
}

/***********************************************************************************************************
 * The following functions are triggered when an appropriate string is inserted by the user.
 * Each function gets a pointer to the board pointer so each function can change the board to a new board.
 * A Command struct is passed to the function which contains the user arguments and function name.
 * Each function should satisfy the following guidelines:
 * 1. Each parameter should be in the correct range -
 *      make sure arguments variables types are correct (for example int instead of string).
 *      make sure arguemnts are in the correct range (for example for threshold between 0.0 to 1.0).
 * 2. The board is not errornous in certain commands.
 * 3. Successful execution of the method.
 * 
 * Each error should be followed by descriptive error message.
 * For concerning errornous use the ERRORNOUS_PRINT string.
 * For error in parameter use PARAMETER_ERROR string.
 * for example:
 *      printf(PARAMETER_ERROR, "1st", "int") -> Error: 1st argument exptected int 
 * 
 * A succsessful command returnes SUCCESS if something went wrong returns COMMAND_FAILED.
***********************************************************************************************************/

/* starts a saved puzzle in solve mode */
int solve_command(Board** b, Command* cmd){
    if(get_file_status(read_file(b, cmd->args[0], SOLVE)) == COMMAND_FAILED)
        return COMMAND_FAILED;
    (*b)->mode = SOLVE;
    return SUCCSESS;
}

/* starts a saved puzzle in edit mode */
int edit_command(Board** b, Command* cmd){
    Board* new_b;
    /* if no param is added a new board is initialized */
    if(cmd->arg_length == 0){
        free_board(*b);
        new_b = malloc(sizeof(Board));
		if (new_b == NULL) {
			printf("Error: malloc has failed\n");
			exit(0);
		}
        init_board(new_b, DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE);
        new_b->mode = EDIT;
        *b = new_b;
    }
    else{
        if(get_file_status(read_file(b, cmd->args[0], EDIT)) == COMMAND_FAILED)
            return COMMAND_FAILED;
        (*b)->mode = EDIT;
    }
    return SUCCSESS;
}

/* Sets the mark errors flag in the board */
int mark_errors_command(Board** b, Command* cmd){
    int flag = 0;
    int arg = check_if_int(cmd->args[0], &flag);
    if(flag == NOT_INT){
        printf(PARAMETER_ERROR, "1st", "int");
        return COMMAND_FAILED;
    }
    if (arg == NO_MARK_ERRORS){
        (*b)->mark_errors = NO_MARK_ERRORS;
    }
    else if (arg == MARK_ERRORS) {
        (*b)->mark_errors = MARK_ERRORS; 
    }
    else {
        printf("Error: first agument should be either %d or %d\n", NO_MARK_ERRORS, MARK_ERRORS);
        return COMMAND_FAILED;
    }
    return SUCCSESS;
}

/* prints the board */
int print_board_command(Board** b, Command* cmd){
    printBoard(*b);
    UNUSED(cmd);
    return SUCCSESS;
}

/* sets a cell to inputted value */
int set_command(Board** b, Command* cmd){
	int* args;
	int i;
	int* flags = calloc(cmd->arg_length, sizeof(int));
	if (flags == NULL) {
		printf("Error: calloc has failed\n");
		exit(0);
	}
	args = set_params_int(cmd, flags);
	for (i = 0; i < cmd->arg_length; i++) {
		if (flags[i] == NOT_INT) {
			free(args);
            free(flags);
			return COMMAND_FAILED;
		}
	}
    free(flags);
	if (out_of_range(*b, args)) {
		printf(ARGS_OUT_OF_RANGE, (*b)->size);
        free(args);
		return COMMAND_FAILED;
	}

    new_commit(*b);
	if (set_cell(*b, args[0] - 1, args[1] - 1, args[2])) {
        finish_commit(*b);
        free(args);
		return SUCCSESS;
	}
    finish_commit(*b);
    free(args);
    printf("Error: cell is fixed\n");
	return COMMAND_FAILED;
}

/* checks if the board is solvable */
int validate_command(Board** b, Command* cmd){
    if(errornous_check(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    if(validate_board(*b) == SOLUTION_FOUND)
        printf("The board is solvable\n");
    else
        printf("The board is unsolvable\n");
    return SUCCSESS;
}

/* guesses the board using a threshold */
int guess_command(Board** b, Command* cmd){
    int flag = 0;
    float val = check_if_float(cmd->args[0], &flag);
    if(flag == NOT_FLOAT){
        printf(PARAMETER_ERROR, "1st", "float");
        return COMMAND_FAILED;
    }
    if(val < 0 || val > 1.0){
        printf(PARAMETER_ERROR, "1st", "a number between 0.0 and 1.0");
        return COMMAND_FAILED;
    }
    if(errornous_check(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    guess_board(*b, atof(cmd->args[0]));
    return SUCCSESS;
}

/* generates a board using ILP */
int generate_command(Board** b, Command* cmd){
    int flag = 0;
    int second_arg, first_arg = check_if_int(cmd->args[0], &flag);
    int max_cells = 0;
    if(flag == NOT_INT){
        printf(PARAMETER_ERROR, "1st", "int");
        return COMMAND_FAILED;
    }
    if (first_arg < 0){
        printf(PARAMETER_ERROR, "1st", "non-negative value");
        return COMMAND_FAILED;
    }
    max_cells = (*b)->size * (*b)->size - (*b)->filled_cells;
    if (max_cells - first_arg < 0){
        printf("Error: in 1st parameter expected up to %d cells\n", max_cells);
        return COMMAND_FAILED;
    }
    /* check for X empty cells */
    second_arg = check_if_int(cmd->args[1], &flag);
    if(flag == NOT_INT){
        printf(PARAMETER_ERROR, "2nd", "int");
        return COMMAND_FAILED;
    }
    if (second_arg <= 0){
        printf(PARAMETER_ERROR, "2nd", "positive value");
        return COMMAND_FAILED;
    }
    if (second_arg > (*b)->size * (*b)->size){
        printf(PARAMETER_ERROR, "2nd", "size less than number of cells");
        return COMMAND_FAILED;
    }
    if(errornous_check(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    if(generate_using_ILP(*b, first_arg, second_arg) == NO_SOLUTION_FOUND){
        printf("Error: puzzle cannot be generated\n");
        return COMMAND_FAILED;
    }
    return SUCCSESS;
}

/* undo last command */
int undo_command(Board** b, Command* cmd){
    UNUSED(cmd);
	if (undo(*b, 0)) {
		return SUCCSESS;
	}
	printf("There are no moves to undo\n");
    return COMMAND_FAILED;
}

/* redo last command */
int redo_command(Board** b, Command* cmd){
    UNUSED(cmd);
	if (redo(*b, 0)) {
		return SUCCSESS;
	}
	printf("There are no moves to redo\n");
	return COMMAND_FAILED;
}

/* saves the board */
int save_command(Board** b, Command* cmd){
    int status = 0;
    if((*b)->mode == EDIT && errornous_check(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    status = get_file_status(save_board(*b, cmd->args[0]));
    if (status == COMMAND_FAILED)
            return COMMAND_FAILED;

    printf("File saved succsessfully\n");
    return SUCCSESS;
}

/* gives a hint to a cell using ILP */
int hint_command(Board** b, Command* cmd){
    if(check_coordinates(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    printf("Set cell to %d\n", ILP_hint(*b, atoi(cmd->args[0]) - 1, atoi(cmd->args[1]) - 1));
    return SUCCSESS;
}

/* gives a hint to a cell using LP */
int guess_hint_command(Board** b, Command* cmd){
    if(check_coordinates(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    guess_hint(*b, atoi(cmd->args[0]) - 1, atoi(cmd->args[1]) - 1);
    return SUCCSESS;
}

/* prints the number of solutions */
int num_solutions_command(Board** b, Command* cmd){
    if(errornous_check(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
    UNUSED(cmd);
	printf("There are %d possible solutions to this borad\n", num_of_solutions(*b));
    return SUCCSESS;
}

/* autofills obvious cells */
int autofill_command(Board** b, Command* cmd){
    if(errornous_check(*b, cmd) == COMMAND_FAILED)
        return COMMAND_FAILED;
	if (autofill(*b) == NO_FILL){
        printf("No cells to fill\n");
        return COMMAND_FAILED;
    }
	return SUCCSESS;
	
}

/* resets board to initial state */
int reset_command(Board** b, Command* cmd){
    UNUSED(cmd);
	reset(*b);
    return SUCCSESS;
}

/* exits the game */
int exit_command(Board** b, Command* cmd){
    printf("Exiting...\n");
    if(*b)
        free_board(*b);
    exit(0);
    UNUSED(cmd);
    return -1;
}

/******************************************************************************
 * User_Command contains each command available to the user.
 * using an array it is possible to iterate over each command.
*****************************************************************************/
typedef struct {
    char* name;
    int optional_arg;
    int (*func)(Board** board_pointer, Command* cmd);
    int args_num;
    int available_in_solve;
    int available_in_edit;
    int available_in_init;
    int print_after;
} User_Command;

User_Command commands[] = {
/*  Command String:        Activated Function       arguments and print_after
 *                                                  modes (S, E, I)
 */
    {"solve",           0, solve_command,           1, 1, 1, 1, 1},
    {"edit",            1, edit_command,            1, 1, 1, 1, 1},
    {"mark_errors",     0, mark_errors_command,     1, 1, 0, 0, 1},
    {"print_board",     0, print_board_command,     0, 1, 1, 0, 0},
    {"set",             0, set_command,             3, 1, 1, 0, 1},
    {"validate",        0, validate_command,        0, 1, 1, 0, 0},
    {"guess",           0, guess_command,           1, 1, 0, 0, 1},
    {"generate",        0, generate_command,        2, 0, 1, 0, 1},
    {"undo",            0, undo_command,            0, 1, 1, 0, 1},
    {"redo",            0, redo_command,            0, 1, 1, 0, 1},
    {"save",            0, save_command,            1, 1, 1, 0, 0},
    {"hint",            0, hint_command,            2, 1, 0, 0, 0},
    {"guess_hint",      0, guess_hint_command,      2, 1, 0, 0, 0},
    {"num_solutions",   0, num_solutions_command,   0, 1, 1, 0, 0},
    {"autofill",        0, autofill_command,        0, 1, 0, 0, 1},
    {"reset",           0, reset_command,           0, 1, 1, 0, 1},
    {"exit",            0, exit_command,            0, 1, 1, 1, 0}
};

/* Used when a command failes due to not being in the correct mode
 * and prints a descriptive information about where the command is available. */
void print_available_modes(User_Command* uc){
    int add_delim = 0;
    if(uc->available_in_init){
        add_delim = 1;
        printf("init mode");
    }
    if(uc->available_in_solve){
        if(add_delim)
            printf(", ");
        add_delim = 1;
        printf("solve mode");
    }
    if(uc->available_in_edit){
        if(add_delim)
            printf(", ");
        add_delim = 1;
        printf("edit mode");
    }
    printf(".\n");
}

/* Checks if the following command is a available in a mode */
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
    printf("Error: %s not available in current mode it is available in ", uc->name);
    print_available_modes(uc);
    return COMMAND_FAILED;
}


/* Checks for the correct number of params */
int check_args_num(User_Command* uc, int args){
        if((!uc->optional_arg && args != uc->args_num) || (uc->optional_arg && args > uc->args_num)){
            printf("Error: %s takes %d arguments\n", uc->name, uc->args_num);
            return COMMAND_FAILED;
        }
    return SUCCSESS;
}

/******************************************************************************
 * This function itereates over each of the options for user command
 * it find the correct command checks if the command is available in the current mode,
 * checks the number of paramters passed and the starts command execution.
*****************************************************************************/
int execute_command(Board** board_pointer){
    Board* b = *board_pointer;
    int i, mode = INIT;
    User_Command uc;
    Command user_command;
    Command* cmd;
    int command_found = 0;
    printf("Enter next command:\n");
    user_input(&user_command);
    cmd = &user_command;

    /* exits on eof */
    if (feof(stdin)){
        exit_command(board_pointer, cmd);
    }
    /* check if input is too long */
    if(cmd->c_status == TOO_MUCH_CHARS) {
        printf("Error: input too long expected up to 256 chars\n");
        return COMMAND_FAILED;
    }

    /* ignoring empty command */
    if(!cmd->name)
        return SUCCSESS;
    for(i = 0; i < COMMANDS_NUM; i++){
        uc = commands[i];
        command_found = 0;
        /* check if inputted command exists */
        if(!strcmp(cmd->name, uc.name)){
            command_found = 1;
            if(b != NULL)
                mode = b->mode;
            /* check if command is available in current mode */
            if(available_command(&uc, mode) == COMMAND_FAILED){
                return COMMAND_FAILED;
            }
            /* check number of arguments */
            if (check_args_num(&uc, cmd->arg_length) == COMMAND_FAILED){
                return COMMAND_FAILED;
            }
            /* stages 3-5 of checking are handled in the following funcion */
            if(uc.func(board_pointer, cmd) == SUCCSESS){
                if(uc.print_after)
                    printBoard(*board_pointer);
                return SUCCSESS;
            }
            else
                return COMMAND_FAILED;
        }
    }
    if (command_found == 0){
        printf("Error: invalid command\n");
        return COMMAND_FAILED;
    }
    return COMMAND_EXECUTED;
}
