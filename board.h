#ifndef BOARD_H
#define BOARD_H

#define BOARD_SETTING_ERROR -1
#define BOARD_SET 1
#define INIT 1
#define EDIT 2
#define SOLVE 3
#define MARK_ERRORS 1
#define NO_MARK_ERRORS 0
#define RECORD 1
#define FIXED_CELL 0
#define SUCCESS 1
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
    int commited;
	Move* head;
    Move* tail;
	struct Moves_Bundle* next;
	struct Moves_Bundle* prev;
} Moves_Bundle;

typedef struct {
    int **solution;
    int **state;
    int **fixed;
    int **wrong;
    int width;
    int height;
    int size;
    int mode;
    int correct_cells;
    int wrong_cells;
    int mark_errors;
	Moves_Bundle* movePointer;
} Board;



void init_board(Board* b, int width, int height);
void free_board(Board* b);
int valid_set_value(Board* b, int x, int y, int val);
int get_options_array(Board* b, int i, int j, int** arr);
int generate_random_cells(Board* b, int x);
void reset_board_state(Board* b);
void generate_from_solution(Board* b, Board* solved, int x);
void autofill(Board* b);
int set_cell(Board* b, int x, int y, int val);
void create_board_copy(Board* orig, Board* new_b);
void free_set_cell(Board* b, int x, int y, int val);


#endif
