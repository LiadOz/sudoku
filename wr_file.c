#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#define READ_MAX_SIZE 3

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

/* Reads a file and creates a new board according to it */
int read_file(Board* b, char file_path[]){
    FILE *fptr;
    int height, width;
    int i, j;
    char temp[READ_MAX_SIZE];
    char* point_ptr;
    fptr = fopen(file_path, "r");
    if(fptr == NULL){
        printf("Error opening file");
        return -1;
    }
    fscanf(fptr, "%d", &height);
    fscanf(fptr, "%d", &width);

    init_board(b, width, height);
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            fscanf(fptr, "%s", temp);
            point_ptr = strchr(temp, '.');
            if(point_ptr == NULL)
                b->state[i][j] = atoi(temp);
            else {
                *point_ptr = '\0';
                b->state[i][j] = atoi(temp);
                b->fixed[i][j] = 1;
            }
        }
    }

    fclose(fptr);
    return 1;
}
