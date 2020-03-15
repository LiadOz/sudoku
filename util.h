#ifndef UTIL_H_
#define UTIL_H_
#include "parsing.h"

#define NOT_INT 1
#define NOT_FLOAT 1


int check_if_int(char* s, int* flag);
float check_if_float(char* s, int* flag);
int* set_params_int(Command* cmd, int* flags);

#endif /* end of include guard: UTIL_H_ */
