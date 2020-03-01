/*
 * Module that uses gurobi to find solution to sudoku board using either ILP or LP.
 * validate_board - uses ILP to determine whether a board has a solution.
 * guess_board - uses LP fill values in board that are over certain threshold.
 * ILP_hint - uses LP to give hint to a certain cell.
 * guess_hint - uses LP to give hint to a cell value.
 *
 * If a board does not have a solution NO_SOLUTION_FOUND is returned.
 * If a board has a solution SOLUTION_FOUND is returned.
 */
#ifndef LP_SOLVER_H 
#define LP_SOLVER_H 
#include "board.h"

#define SOLUTION_FOUND -1
#define NO_SOLUTION_FOUND -2

int validate_board(Board* b);
int guess_board(Board* b, double thresh);
int ILP_hint(Board* b, int i, int j);
int guess_hint(Board* b, int i, int j);
int generate_using_ILP(Board* b, int x, int y);

#endif
