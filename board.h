#ifndef BOARD_H
#define BOARD_H

typedef struct {
    int **solution;
    int **state;
    int **fixed;
    int width;
    int height;
    int size;
} Board;
void init_board(Board* b, int width, int height);
void free_board(Board* b);
void set_cell(Board* b, int x, int y, int val);
void hint(Board* b, int x, int y);
void printBoard(Board* b);

#endif
