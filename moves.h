/*
* This module is responsible for document and record the real-time changes and moves in the board during the game.
*
* ******      Data Structures:      ******
* -Bundle: a set of moves that executed under the same command.
* -Move: struct that contain details about a change in a single cell.
*
* ******      Functions:      ******
* create_new_move - creates a new move and returns a pointer to it.
* print_change - given a move and a command(undo/redo), prints the change made by this command.
* exec_moves - given a board, command and a reset flag notation, the function changes the board according to the command,
*			   if reset flag is on, the function is not printing the changes.
* undo - undo the last change(s) in the board.
* redo - redo the most recent change(s) in the board.
* reset - reset the board to starting point.
* free_all_moves - frees all moves before freesing the board itself.
* new_commit - creating a bundle to store the next moves. 
* add_move - adds a move to last uncommited bundle.
* finish_commit - closes the commit to the bundle.
*/

#ifndef MOVES_H
#define MOVES_H

#define UNDO_COMMAND "undo"
#define REDO_COMMAND "redo"


#define COMMITED 1

Move* create_new_move(Board* b, int x, int y, int val);
void print_change(char* command, Move* move);
void exec_moves(Board* b, char* command, int reset);
int undo(Board* b, int reset);
int redo(Board* b, int reset);
void reset(Board* b);
void free_all_moves(Board* b);
void new_commit(Board* b);
void add_move(Board* b, int x, int y, int val);
void finish_commit(Board *b);

#endif
