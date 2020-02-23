#ifndef POSSIBLE_TABLE_H
#define POSSIBLE_TABLE_H
#include "board.h"

/* Each entry summarizes how what numbers can go in each cell of the board
 * the start_index and end_index tells where these numbers are located in
 * the EntryTable object */
typedef struct{
    int value;
    int* valid_nums;
    int count;
    int start_index;
    int end_index;
} PossibleEntry;

typedef struct{
    PossibleEntry** entries;
    int* var_arr;
    int var_count;
    int size;
    int block_width;
    int block_height;
} EntryTable;

int init_entry_table(EntryTable* et, Board* b);
void free_entry_table(EntryTable* et);
#endif