#ifndef WR_FILE
#define WR_FILE
#include "board.h"

#define FILE_FORMAT_ERROR 1
#define FILE_NOT_FOUND 2
#define FILE_READ 3
#define FILE_UNSOLVABLE 4
#define FILE_SAVED 5

int save_board(Board* b, char file_path[]);
int read_file(Board** old_b, char file_path[]);

#endif
