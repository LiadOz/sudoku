#ifndef WR_FILE
#define WR_FILE
#include "board.h"

#define FILE_FORMAT_ERROR 1

int save_board(Board* b, char file_path[]);
int read_file(Board* b, char file_path[]);

#endif
