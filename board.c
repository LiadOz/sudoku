#include <stdlib.h>
#include <stdio.h>
#include "board.h"

/*
 * used to populate the board
 */
void init_board(Board* b, int width, int height){
    int i;
    int** p;
    int* p2;
    p = malloc(width * height * sizeof(int *));
    if(p == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    b->solution = p;
    
    p = malloc(width * height * sizeof(int *));
    if(p == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    b->state = p;

    p = malloc(width * height * sizeof(int *));
    if(p == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    b->fixed = p;

    for(i = 0; i < width * height; i++){
        p2 = calloc(width * height, sizeof(int));
        if(p2 == NULL){
            printf("Error: calloc has failed\n");
            exit(0);
        }
        b->solution[i] = p2;

        p2 = calloc(width * height, sizeof(int));
        if(p2 == NULL){
            printf("Error: calloc has failed\n");
            exit(0);
        }
        b->state[i] = p2;

        p2 = calloc(width * height, sizeof(int));
        if(p2 == NULL){
            printf("Error: calloc has failed\n");
            exit(0);
        }
        b->fixed[i] = p2;
    }
    b->width = width;
    b->height = height;
    b->size = width * height;
    b->correct_cells = 0;
    /* create random board */
}

/* used to free allocation when program exits */
void free_board(Board* b){
    int i;
    for(i = 0; i < b->size; i++){
        free(b->solution[i]);
        free(b->state[i]);
        free(b->fixed[i]);
    }
    free(b->solution);
    free(b->state);
    free(b->fixed);
}

/* sets the game state to user according to solved board and a number of cells to be keep */
void set_from_solution(Board* b, int empty_cells){
    int r_x, r_y;
    b->correct_cells = empty_cells;
    while(empty_cells > 0){
        r_x = rand() % b->size;
        r_y = rand() % b->size;
        if(b->fixed[r_y][r_x] == 0){
            b->state[r_y][r_x] = b->solution[r_y][r_x];
            b->fixed[r_y][r_x] = 1;
            empty_cells--;
        }
    }
}

/* checks if number exists in array if true return 1 */
int number_in_array(int* arr, int number, int size){
    int i;
    for(i = 0; i < size; i++){
        if(arr[i] == number){
            return 1;
        }
    }
    return 0;
}

/* Returns array of all the numbers in the row
 * assumes row is in the range
 * dynamically allocate memory must be freed
 */
int* get_row_numbers(Board* b, int row){
    int i;
    int *arr = malloc(b->size * sizeof(int));
    if(arr == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    for(i = 0; i < b->size; i++){
        arr[i] = b->state[row][i];
    }
    return arr;
}


/* Returns array of all the numbers in the column 
 * assumes column is in the range
 * dynamically allocate memory must be freed
 */
int* get_column_numbers(Board* b, int col){
    int i;
    int *arr = malloc(b->size * sizeof(int));
    if(arr == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    for(i = 0; i < b->size; i++){
        arr[i] = b->state[i][col];
    }
    return arr;
}

/* returns the block index of coordinates */
int get_coordinate_block_number(Board* b, int x, int y){
    return x / b->height * b->height + y / b->width;
}

/* Returns array of all the numbers in the block
 * blocks are defined from up to bottom left to right
 * the number of blocks is width * height
 * assumes block is in the range
 * dynamically allocate memory must be freed
 */
int* get_block_numbers(Board* b, int block_number){
    int block_y = block_number / b->height;
    int block_x = block_number - (block_y * b->height);
    int start_x = b->width * block_x;
    int start_y = b->height* block_y;
    int i, j, c = 0;
    int *arr = malloc(b->size * sizeof(int));
    if(arr == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    for(i = start_y; i < b->height + start_y; i++){
        for(j = start_x; j < b->width + start_x; j++){
            arr[c++] = b->state[i][j];
        }
    }
    return arr;
}

/* checks if number is in the area if true return 1
 * mode 0 = row
 * mode 1 = column
 * mode 2 = block
 */ 
int number_in_area(Board* b, int mode, int index, int number){
    int* arr;
    int result; 
    if(mode == 0){
        arr = get_row_numbers(b, index);
    }
    else if(mode == 1){
        arr = get_column_numbers(b, index);
    }
    else if(mode == 2){
        arr = get_block_numbers(b, index);
    }
    else {
        return -1;
    }

    result = number_in_array(arr, number, b->size);
    free(arr);
    return result;
}

/* checks if value can be inserted into x y coordinates return 1 if true */
int valid_set_value(Board* b, int x, int y, int val){
    if(!number_in_area(b, 0, x, val) && !number_in_area(b, 1, y, val) && !number_in_area(b, 2, get_coordinate_block_number(b, x, y), val)){
        return 1;
            }
    return 0;
}

/* sets a cell to value with x, y coordinates
 * x and y starts at 1
 * returns 1 when assignment worked
 */
int set_cell(Board* b, int x, int y, int val){
    if(b->fixed[x][y] == 1){
        printf("Error: cell is fixed\n");
        return 0;
    }
    if(b->state[x][y] == val){
        return 1;
    }
    if(valid_set_value(b, x, y, val)){
        /* if the user added a new number the number of correct_cells goes up
         * if the user removed a number the number of correct_cells goes down */
        if(b->state[x][y] == 0 && val != 0){
            b->correct_cells++;
        }
        if(b->state[x][y] != 0 && val == 0){
            b->correct_cells--;
        }
        b->state[x][y] = val;
        return 1;
    }
    else{
        printf("Error: value is invalid\n");
        return 0;
    }
}

/* gives hint to cell (x, y) when they start at 1 */
void hint(Board* b, int x, int y){
    printf("Hint: set cell to %d\n", b->solution[x][y]);
}
