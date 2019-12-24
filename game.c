#define BLOCK_SIZE 2

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
    int solution[BLOCK_SIZE * BLOCK_SIZE][BLOCK_SIZE * BLOCK_SIZE];
    int state[BLOCK_SIZE * BLOCK_SIZE][BLOCK_SIZE * BLOCK_SIZE];
    int fixed[BLOCK_SIZE * BLOCK_SIZE][BLOCK_SIZE * BLOCK_SIZE];
} Board;

int main(){
    Board b1;
    sample_board(&b1);
    printf("state = %d solution = %d\n", b1.state[0][0], b1.solution[0][0]);
    printf("state = %d solution = %d\n", b1.state[3][0], b1.solution[3][0]);
    return 0;
}
/*
 * used to populate the board
 */
void init_board(Board* p){
}

/*
 * create simple board for dubgging
 * should look like:
 * 1 2 3 4
 * 3 4 1 2
 * 2 1 4 3
 * 0 0 0 0
 *
 */
void sample_board(Board* p){
    p->solution[0][0] = 1;
    p->solution[0][1] = 2;
    p->solution[0][2] = 3;
    p->solution[0][3] = 4;
    p->solution[1][0] = 3;
    p->solution[1][1] = 4;
    p->solution[1][2] = 2;
    p->solution[1][3] = 1;
    p->solution[2][0] = 2;
    p->solution[2][1] = 1;
    p->solution[2][2] = 4;
    p->solution[2][3] = 3;
    p->solution[3][0] = 4;
    p->solution[3][1] = 3;
    p->solution[3][2] = 2;
    p->solution[3][3] = 1;

    p->fixed[0][0] = 1;
    p->fixed[0][1] = 1;
    p->fixed[0][2] = 1;
    p->fixed[0][3] = 1;
    p->fixed[1][0] = 1;
    p->fixed[1][1] = 1;
    p->fixed[1][2] = 1;
    p->fixed[1][3] = 1;
    p->fixed[2][0] = 1;
    p->fixed[2][1] = 1;
    p->fixed[2][2] = 1;
    p->fixed[2][3] = 1;
    p->fixed[3][0] = 0;
    p->fixed[3][1] = 0;
    p->fixed[3][2] = 0;
    p->fixed[3][3] = 0;

    p->state[0][0] = 1;
    p->state[0][1] = 2;
    p->state[0][2] = 3;
    p->state[0][3] = 4;
    p->state[1][0] = 3;
    p->state[1][1] = 4;
    p->state[1][2] = 2;
    p->state[1][3] = 1;
    p->state[2][0] = 2;
    p->state[2][1] = 1;
    p->state[2][2] = 4;
    p->state[2][3] = 3;
    p->state[3][0] = 0;
    p->state[3][1] = 0;
    p->state[3][2] = 0;
    p->state[3][3] = 0;
}
