#include <stdio.h>
#include <stdlib.h>
#include "possible_table.h"
#include "board.h"
/* takes a board state and create the constraints and variables needed for gurobi */

int init_entry_table(EntryTable* et, Board* b){
    /* for each cell we want to check if number can be inputted
     * if so we add the number and cell as a new variable */
    int i, j, k, cell;
    PossibleEntry** options;
    int vars = 0;
    int* var_arr;
    et->size = b->size;
    et->block_width = b->width;
    et->block_height = b->height;
    var_arr = malloc(b->size * b->size * b->size * sizeof(int));
    options = malloc(b->size * sizeof(PossibleEntry *));
    if(options == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    for(i = 0; i < b->size; i++){
        options[i] = malloc(b->size * sizeof(PossibleEntry));
        if(options[i] == NULL){
            printf("Error: malloc has failed\n");
            exit(0);
        }
    }
    /* setting each entry inside the table */
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            PossibleEntry temp;
            cell = b->state[i][j];
            temp.value = cell;
            if(cell == 0){
                temp.value= 0;
                temp.count = get_options_array(b, i, j, &temp.valid_nums);
                temp.start_index = vars;
                for(k = 0; k < temp.count; k++){
                    var_arr[vars] = temp.valid_nums[k];
                    vars++;
                }
                temp.end_index = vars+1;
            }
            options[i][j] = temp;
        }
    }
    et->entries = options;
    var_arr = realloc(var_arr, vars * sizeof(int));
    et->var_arr = var_arr;
    et->var_count = vars;
    return 1;
}

void free_possible_entry(PossibleEntry* pe){
    free(pe->valid_nums);
}

void free_entry_table(EntryTable* et){
    int i, j;
    for(i = 0; i < et->size; i++){
        for(j = 0; j < et->size; j++){
            if(!et->entries[i][j].value){
                free_possible_entry(&et->entries[i][j]);
            }
        }
        free(et->entries[i]);
    }
    free(et->entries);
    free(et->var_arr);
}

