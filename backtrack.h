/*
* Module that finds the number of possible solutions of a sudoku board.
* The module uses a calling stack by using determinisitic backtracking algorithem.
*
* ******      Data Structurs:      ******
* Stack - a stack of nodes that contain the data about a single cell.
*
* ******      Functions:      ******
* num_solutions - implementing the purpose of this module, finds the number of possible solutions of a sudoku board.
*/

#ifndef BACKTRACK_H
#define BACKTRACK_H

#include "board.h"

int num_of_solutions(Board* b);

#endif
