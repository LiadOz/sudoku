#ifndef MOVES_H
#define MOVES_H

#define COMMITED 1

Move* create_new_move(Board* b, int x, int y, int val);
void add_moves_to_board(Board* b, Move* head);
void exec_moves(Board* b, char* command, int reset);
int undo(Board* b, int reset);
int redo(Board* b, int reset);
void print_change(char* command, Move* move);
void reset(Board* b);
void free_all_moves(Board* b);



void new_commit(Board* b);
void add_move(Board* b, int x, int y, int val);
void finish_commit(Board *b);

#endif
