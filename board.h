#ifndef BOARD_H
#define BOARD_H

#define BOARD_SETTING_ERROR -1
#define BOARD_SET 1
/*
 * Board struct:
 * solution - the last solution given by the validate method
 * state - the state of the board with user guesses
 * fixed - determine wheter a cell is fixed (1) or not (0)
 * correct_cells - the number of cells inputted correctly
 */

typedef struct Move{
	int x;
	int y;
	int currVal;
	int prevVal;
	struct Move* next;
} Move;

typedef struct Moves_Bundle {
	int first;
	Move* head;
	struct Moves_Bundle* next;
	struct Moves_Bundle* prev;
} Moves_Bundle;

typedef struct {
    int **solution;
    int **state;
    int **fixed;
    int width;
    int height;
    int size;
    int correct_cells;
	Moves_Bundle* movePointer;
} Board;



void init_board(Board* b, int width, int height);
void free_board(Board* b);
int set_cell(Board* b, int x, int y, int val, Move** set);
void hint(Board* b, int x, int y);
void sample_board(Board* p);
void set_from_solution(Board* b, int empty_cells);
int valid_set_value(Board* b, int x, int y, int val);
int get_options_array(Board* b, int i, int j, int** arr);
int generate_random_cells(Board* b, int x);
void reset_board_state(Board* b);
void generate_from_solution(Board* b, int x);
int autofill(Board* b, Move** head);

#endif
