#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "parsing.h"
#include "printing.h"

/* Returns NOT_INT if is not int
 * if it is int it returns the number */
int check_if_int(char* s, int* flag){
    int num = atoi(s);
    if(num == 0 && s[0] != '0'){
        *flag = NOT_INT;
        return 0;
    }
    return num;
}

/* Returns NOT_FLOAT if is not float
 * if it is int it returns the number */
float check_if_float(char* s, int* flag){
    float val;
    if(sscanf(s, "%f", &val) != 0)
        return val;
    *flag = NOT_FLOAT;
    return 0;
}

int* set_params_int(Command* cmd, int* flags) {
	int* args = (int*)malloc(cmd->arg_length * sizeof(int));
	int i;
	for (i = 0; i < cmd->arg_length; i++) {
		args[i] = check_if_int(cmd->args[i], &(flags[i]));
		if (flags[i] == NOT_INT) {
			if (i == 0) {
				printf(PARAMETER_ERROR, "1st", "int");
			}
			else if (i == 1) {
				printf(PARAMETER_ERROR, "2nd", "int");
			}
			else {
				printf(PARAMETER_ERROR, "3rd", "int");
			}
		}
	}
	return args;
}
