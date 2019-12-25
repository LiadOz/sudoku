#define BLOCK_WIDTH 2
#define BLOCK_HEIGHT 3

#include <stdlib.h>
/*
 * temp include for debugging
 */
#include <stdio.h>

/*
 * Board struct:
 * solution - the last solution given by the validate method
 * state - the state of the board with user guesses
 * fixed - determine wheter a cell is fixed (1) or not (0)
 */
typedef struct {
    int solution[BLOCK_WIDTH * BLOCK_HEIGHT][BLOCK_WIDTH * BLOCK_HEIGHT];
    int state[BLOCK_WIDTH * BLOCK_HEIGHT][BLOCK_WIDTH * BLOCK_HEIGHT];
    int fixed[BLOCK_WIDTH * BLOCK_HEIGHT][BLOCK_WIDTH * BLOCK_HEIGHT];
} Board;

/*
 * used to populate the board
 */
void init_board(Board* p, int width, int height){
    /* dynamically allocate memory to struct */
    /* create random board */
}

/* used to free allocation when program exits */
void free_board(Board* p){
    /* free up resources */
}

/* Returns array of all the numbers in the row
 * assumes row is in the range
 */
int* get_row_numbers(Board* b, int row){
    return b->state[row];
}

/* Returns array of all the numbers in the column 
 * assumes column is in the range
 * dynamically allocates memory
 */
int* get_column_numbers(Board* b, int col){
    /* needs to be allocated and freed */
    int *arr = malloc(BLOCK_HEIGHT * BLOCK_WIDTH * sizeof(int));
    int i;
    for(i = 0; i < BLOCK_HEIGHT * BLOCK_WIDTH; i++){
        arr[i] = b->state[i][col];
    }
    return arr;
}

/* Returns array of all the numbers in the block
 * blocks are defined from up to bottom left to right
 * the number of blocks is width * height
 * assumes block is in the range
 * dynamically allocates memory
 */
int* NOTWORKINGYETget_block_numbers(Board* b, int block_number){
    /* needs to be allocated and freed */
    int *arr = malloc(BLOCK_HEIGHT * BLOCK_WIDTH * sizeof(int));
    int i, j, count = 0;
    for(i = 0; i < BLOCK_HEIGHT; i++){
        for(j = 0; j < BLOCK_WIDTH; j++){
            arr[count++] = b->state[i][j];
        }
    }
    return arr;
}

/*
 * create simple board for dubgging
 * initial state should look like:
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

int main(){
    Board b1;
    sample_board(&b1);
    int* arr = get_row_numbers(&b1, 1);
    for(int i = 0; i < 6; i++){
        printf("%d ", arr[i]);
    }
    free(arr);
    printf("\n");
    return 0;
}
