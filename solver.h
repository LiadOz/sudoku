#ifndef SOLVER_H
#define SOLVER_H

int createSolution(int i, int j, Board* b);
int solveByDBT(int i, int j, Board* b);
int validate(int i, int j, Board* b);
int get_i_element(int* arr, int arr_size, int i);

#endif
