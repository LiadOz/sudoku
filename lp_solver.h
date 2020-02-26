#ifndef LP_SOLVER_H 
#define LP_SOLVER_H 
#include "board.h"

#define SOLUTION_FOUND -1
#define NO_SOLUTION_FOUND -2

int validate_board(Board* b);
int guess_board(Board* b, double thresh);
int ILP_hint(Board* b, int i, int j);
void test();

#endif
