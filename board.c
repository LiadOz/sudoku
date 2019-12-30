#include <stdlib.h>
#include <stdio.h>
#include "board.h"

void sample_board(Board* p);

/*
 * used to populate the board
 */
void init_board(Board* b, int width, int height){
    int i;
    int** p;
    int* p2;
    p = malloc(width * height * sizeof(int *));
    if(p == NULL){
        printf("Error: init_board has failed\n");
        exit(0);
    }
    b->solution = p;
    
    p = malloc(width * height * sizeof(int *));
    if(p == NULL){
        printf("Error: init_board has failed\n");
        exit(0);
    }
    b->state = p;

    p = malloc(width * height * sizeof(int *));
    if(p == NULL){
        printf("Error: init_board has failed\n");
        exit(0);
    }
    b->fixed = p;

    for(i = 0; i < width * height; i++){
        p2 = calloc(width * height, sizeof(int));
        if(p2 == NULL){
            printf("Error: init_board has failed\n");
            exit(0);
        }
        b->solution[i] = p2;

        p2 = calloc(width * height, sizeof(int));
        if(p2 == NULL){
            printf("Error: init_board has failed\n");
            exit(0);
        }
        b->state[i] = p2;

        p2 = calloc(width * height, sizeof(int));
        if(p2 == NULL){
            printf("Error: init_board has failed\n");
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
        printf("Error: get_row_numbers has failed\n");
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
        printf("Error: get_column_numbers has failed\n");
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
        printf("Error: get_block_numbers has failed\n");
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
/*
int main(){
    Board b;
    int i;
    init_board(&b, 2, 3);
    sample_board(&b);
    printf("%d\n", get_coordinate_block_number(&b, 3, 4));
    int* arr = get_block_numbers(&b, 5); 
    for(i = 0; i < 6; i++){
        printf("%d\n", arr[i]);
    }
    return 1;
}
*/

/*
 * create simple board for dubgging
 * initial state should look like:
 * size 2, 3
 * 1 2 | 3 4 | 5 6
 * 3 4 | 5 6 | 1 2
 * 5 6 | 1 2 | 3 4
 * ---------------
 * 2 1 | 5 6 | 0 0
 * 4 3 | 2 1 | 0 0
 * 6 5 | 4 3 | 0 0
 *
 */
void sample_board(Board* p){
    p->solution[0][0] = 1;
    p->solution[0][1] = 2;
    p->solution[0][2] = 3;
    p->solution[0][3] = 4;
    p->solution[0][4] = 5;
    p->solution[0][5] = 6;
    p->solution[1][0] = 3;
    p->solution[1][1] = 4;
    p->solution[1][2] = 5;
    p->solution[1][3] = 6;
    p->solution[1][4] = 1;
    p->solution[1][5] = 2;
    p->solution[2][0] = 5;
    p->solution[2][1] = 6;
    p->solution[2][2] = 1;
    p->solution[2][3] = 2;
    p->solution[2][4] = 3;
    p->solution[2][5] = 4;
    /* second half */
    p->solution[3][0] = 2;
    p->solution[3][1] = 1;
    p->solution[3][2] = 5;
    p->solution[3][3] = 6;
    p->solution[3][4] = 4;
    p->solution[3][5] = 3;
    p->solution[4][0] = 4;
    p->solution[4][1] = 3;
    p->solution[4][2] = 2;
    p->solution[4][3] = 1;
    p->solution[4][4] = 6;
    p->solution[4][5] = 5;
    p->solution[5][0] = 6;
    p->solution[5][1] = 5;
    p->solution[5][2] = 4;
    p->solution[5][3] = 3;
    p->solution[5][4] = 2;
    p->solution[5][5] = 1;

    p->state[0][0] = 1;
    p->state[0][1] = 2;
    p->state[0][2] = 3;
    p->state[0][3] = 4;
    p->state[0][4] = 5;
    p->state[0][5] = 6;
    p->state[1][0] = 3;
    p->state[1][1] = 4;
    p->state[1][2] = 5;
    p->state[1][3] = 6;
    p->state[1][4] = 1;
    p->state[1][5] = 2;
    p->state[2][0] = 5;
    p->state[2][1] = 6;
    p->state[2][2] = 1;
    p->state[2][3] = 2;
    p->state[2][4] = 3;
    p->state[2][5] = 4;
    p->state[3][0] = 2;
    p->state[3][1] = 1;
    p->state[3][2] = 5;
    p->state[3][3] = 6;
    p->state[4][0] = 4;
    p->state[4][1] = 3;
    p->state[4][2] = 2;
    p->state[4][3] = 1;
    p->state[5][0] = 6;
    p->state[5][1] = 5;
    p->state[5][2] = 4;
    p->state[5][3] = 3;

    p->fixed[0][0] = 1;
    p->fixed[0][1] = 1;
    p->fixed[0][2] = 1;
    p->fixed[0][3] = 1;
    p->fixed[0][4] = 1;
    p->fixed[0][5] = 1;
    p->fixed[1][0] = 1;
    p->fixed[1][1] = 1;
    p->fixed[1][2] = 1;
    p->fixed[1][3] = 1;
    p->fixed[1][4] = 1;
    p->fixed[1][5] = 1;
    p->fixed[2][0] = 1;
    p->fixed[2][1] = 1;
    p->fixed[2][2] = 1;
    p->fixed[2][3] = 1;
    p->fixed[2][4] = 1;
    p->fixed[2][5] = 1;
    p->fixed[3][0] = 1;
    p->fixed[3][1] = 1;
    p->fixed[3][2] = 1;
    p->fixed[3][3] = 1;
    p->fixed[3][4] = 0;
    p->fixed[3][5] = 0;
    p->fixed[4][0] = 1;
    p->fixed[4][1] = 1;
    p->fixed[4][2] = 1;
    p->fixed[4][3] = 1;
    p->fixed[4][4] = 0;
    p->fixed[4][5] = 0;
    p->fixed[5][0] = 1;
    p->fixed[5][1] = 1;
    p->fixed[5][2] = 1;
    p->fixed[5][3] = 1;
    p->fixed[5][4] = 0;
    p->fixed[5][5] = 0;
}
