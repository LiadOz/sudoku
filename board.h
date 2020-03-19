/* 
 * This module houses the core logic and structure of the board at any given moment
 * it has the main functions to manipulate the board.
 *
 * get_options_array - returns an allocated array with every possible value to a given cell.
 * valid_set_value - indicates whether a cell can be set to a value.
 * set_cell - sets cell to a value if it isn't fixed.
 * free_set_cell - sets a cell to a value without checking fixed cell.
 *
 * create_board_copy - creates a copy of the board.
 * generate_random_cells - randomly fills cells with x values.
 * generate_from_solution - generates a board by removing cells from a solved board.
 * autofill - autofills obvious cells.
 * reset_board_state - resets board to 0's.
 */

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
#define NO_FILL 1

/* A node in Doubly Linked List of moves */
typedef struct Move{
	int x;
	int y;
	int currVal;
	int prevVal;
	struct Move* next;
} Move;

/* 
 * Represents a node in Doubly Linked List
 * Each of these nodes contains a Doubly Linked List of moves commited in the current node
 * so each node represents a bundle of changes to the board.
 * If the user used the set command which changes at most 1 cell then the node will have
 * only one move in the Linked List.
 * If the user used the autofill command the node will have the entire list of moves committed
 * by the autofill command.
 * 
 * The commit variable tells whether or not more moves can be appended to the bundle.
 */
typedef struct Moves_Bundle {
	int first;
    int commited;
	Move* head;
    Move* tail;
	struct Moves_Bundle* next;
	struct Moves_Bundle* prev;
} Moves_Bundle;

/*
 * Board struct:
 * movePointer - A Doubly Linked List which contains the previous moves.
 * state - A 2D int array with with user guesses.
 * fixed - Determine wheter a cell is fixed (1) or not (0)
 * filled_cells - The number of cells inputted.
 * wrong_cells - The number of errornous cells.
 * mode - Current mode of the game.
 * mark_errors - Whether or not to show errors.
 */
typedef struct {
    int **state;
    int **fixed;
    int **wrong;
    int width;
    int height;
    int size;
    int mode;
    int filled_cells;
    int wrong_cells;
    int mark_errors;
	Moves_Bundle* movePointer;
} Board;

void init_board(Board* b, int width, int height);
void free_board(Board* b);

int get_options_array(Board* b, int i, int j, int** arr);
int valid_set_value(Board* b, int x, int y, int val);
int set_cell(Board* b, int x, int y, int val);
void free_set_cell(Board* b, int x, int y, int val);

void create_board_copy(Board* orig, Board* new_b);
int generate_random_cells(Board* b, int x);
void generate_from_solution(Board* b, Board* solved, int x);
int autofill(Board* b);
void reset_board_state(Board* b);

#endif
