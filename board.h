#ifndef BOARD_H
#define BOARD_H

/*
 * Board struct:
 * solution - the last solution given by the validate method
 * state - the state of the board with user guesses
 * fixed - determine wheter a cell is fixed (1) or not (0)
 * correct_cells - the number of cells inputted correctly
 */
typedef struct {
    int **solution;
    int **state;
    int **fixed;
    int width;
    int height;
    int size;
    int correct_cells;
} Board;

void init_board(Board* b, int width, int height);
void free_board(Board* b);
int set_cell(Board* b, int x, int y, int val);
void hint(Board* b, int x, int y);
void sample_board(Board* p);
void set_from_solution(Board* b, int empty_cells);
int valid_set_value(Board* b, int x, int y, int val);
int get_options_array(Board* b, int i, int j, int** arr);

#endif
