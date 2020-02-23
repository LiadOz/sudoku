#include <stdio.h>
#include "board.h"

/* save the board state to location 'file_path' */
int save_board(Board* b, char file_path[]){
    FILE *fptr;
    int i, j;
    fptr = fopen(file_path, "w");
    if(fptr == NULL){
        printf("Error opening file");
        return -1;
    }
    /* prints height and width */
    fprintf(fptr, "%d %d\n", b->height, b->width);

    /* prints cells */
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            if(j != 0)
                fprintf(fptr, " ");
            fprintf(fptr, "%d", b->state[i][j]);
            if(b->fixed[i][j])
                fprintf(fptr, ".");
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
    return 1;
}
