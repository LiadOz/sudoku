#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wr_file.h"
#include "board.h"
#include "util.h"
#define READ_MAX_SIZE 3

/* save the board state to location 'file_path' */
int save_board(Board* b, char file_path[]){
    FILE *fptr;
    int i, j;
    fptr = fopen(file_path, "w");
    if(fptr == NULL){
        return FILE_NOT_FOUND;
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
    return FILE_SAVED;
}

/* Reads a file and creates a new board according to it */
int read_file(Board** old_b, char file_path[], int mode){
	FILE* fptr;
	unsigned long pos;
	int height, width;
	int i, j, flag = 0;
	char temp[READ_MAX_SIZE];
	char* point_ptr;
	int val;
    Board* b;

    fptr = fopen(file_path, "r");
    if(fptr == NULL){
        return FILE_NOT_FOUND;
    }
    b = (Board *)malloc(sizeof(Board));
	if (b == NULL) {
		printf("Error: malloc has failed\n");
		exit(0);
	}
    fscanf(fptr, "%d", &height);
    fscanf(fptr, "%d", &width);
    if(height < 1 || width < 1)
        return FILE_FORMAT_ERROR;

    init_board(b, width, height);
    b->mode = mode;
    /* first insert the fixed values and check whether the board is solvable */
    pos = ftell(fptr);
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            /* testing if inputs exists */
            if(fscanf(fptr, "%s", temp) == EOF)
                return FILE_FORMAT_ERROR;
            point_ptr = strchr(temp, '.');
            /* inserting fixed values */
            if(point_ptr != NULL){
                *point_ptr = '\0';
                val = atoi(temp);
                if(val > b->size)
                    return FILE_FORMAT_ERROR;
                else if(!valid_set_value(b, i, j, val))
                    return FILE_UNSOLVABLE;
                set_cell(b, i, j, val);
                b->fixed[i][j] = 1;
            }
        }
    }
    fseek(fptr, pos, 0);
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            fscanf(fptr, "%s", temp);
            point_ptr = strchr(temp, '.');
            if(point_ptr == NULL){
                val = check_if_int(temp, &flag);
                if(flag == NOT_INT)
                    return FILE_FORMAT_ERROR;
                val = atoi(temp);
                if(val > b->size)
                    return FILE_FORMAT_ERROR;
                set_cell(b, i, j, val);
            }
        }
    }
    fclose(fptr);
    if(*old_b){
        free_board(*old_b);
    }
    *old_b = b;
    return FILE_READ;
}
