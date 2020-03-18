#include <stdio.h>
#include <stdlib.h>
#include "possible_table.h"
#include "board.h"
#include "moves.h"
/* takes a board state and create the constraints and variables needed for gurobi */

#define RANDOM_DIGITS 1000

int init_entry_table(EntryTable* et, Board* b){
    /* for each cell we want to check if number can be inputted
     * if so we add the number and cell as a new variable */
    int i, j, k, cell;
    PossibleEntry*** options;
    int vars = 0;
    int* var_arr;
    et->size = b->size;
    et->block_width = b->width;
    et->block_height = b->height;
    var_arr = malloc(b->size * b->size * b->size * sizeof(int));
    options = malloc(b->size * sizeof(PossibleEntry **));
    if(options == NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    for(i = 0; i < b->size; i++){
        options[i] = malloc(b->size * sizeof(PossibleEntry *));
        if(options[i] == NULL){
            printf("Error: malloc has failed\n");
            exit(0);
        }
    }
    /* setting each entry inside the table */
    for(i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            PossibleEntry* temp = malloc(sizeof(PossibleEntry));
            cell = b->state[i][j];
            temp->value = cell;
            if(cell == 0){
                temp->value= 0;
                temp->count = get_options_array(b, i, j, &temp->valid_nums);
                temp->start_index = vars;
                for(k = 0; k < temp->count; k++){
                    var_arr[vars] = temp->valid_nums[k];
                    vars++;
                }
                temp->end_index = vars+1;
                temp->percent = calloc(temp->count, sizeof(double));
            }
            options[i][j] = temp;
            free(temp);
        }
    }
    et->entries = options;
    var_arr = realloc(var_arr, vars * sizeof(int));
    et->var_arr = var_arr;
    et->var_count = vars;
    return 1;
}

/* fills the each entry with the percent of each number in cell */
void insert_solution(EntryTable* et, double* sol){
    PossibleEntry* pt;
    int i, j, k;
    int sol_ptr = 0;
    for(i = 0; i < et->size; i++){
        for(j = 0; j < et->size; j++){
            pt = et->entries[i][j];
            if(!pt->value){
                for(k = 0; k < pt->count; k++){
                    pt->percent[k] = sol[sol_ptr];
                    sol_ptr++;
                }
            }
        }
    }
}

/* prints table used for debugging */
void print_table(EntryTable* et){
    PossibleEntry* pt;
    int i, j, k;
    for(i = 0; i < et->size; i++){
        for(j = 0; j < et->size; j++){
            pt = et->entries[i][j];
            if(!pt->value){
                for(k = 0; k < pt->count; k++){
                    if(i == 0 && j == 0)
                        printf("cell (%d,%d) value %d percent = %1.1f\n", i, j, pt->valid_nums[k], pt->percent[k]);
                }
            }
        }
    }
}

/* returns a random number to insert into cell*/
int choose_random_number(EntryTable* et, Board* b, int i, int j, double thresh){
    int k, min_index;
    double random, sum = 0;
    double* cumlative_prob;
    PossibleEntry* pt = et->entries[i][j];
    cumlative_prob = calloc(pt->count, sizeof(double));
    for(k = 0; k < pt->count; k++){
        /* removing numbers that cannot be inserted anymore and finding the sum of the probabilty */
        if(pt->percent[k] < thresh || !valid_set_value(b, i, j, pt->valid_nums[k])){
            pt->percent[k] = 0;
        }
        sum += pt->percent[k];
        cumlative_prob[k] = sum;
    }

    /* creating random number in range */
    random = (rand() % RANDOM_DIGITS) / (RANDOM_DIGITS / sum);
    if (sum == 0){
        free(cumlative_prob);
        return 0;
    }
    /* determing which number to take */
    for(k = 0; k < pt->count; k++){
        if(cumlative_prob[k] > random){
            min_index = k;
            break;
        }
    }
    free(cumlative_prob);
    return pt->valid_nums[min_index];
}

void fill_with_thresh(EntryTable* et, Board* b, double thresh){
    int i, j, set_value;
    new_commit(b);
    for(i = 0; i < et->size; i++){
        for(j = 0; j < et->size; j++){
            if(b->state[i][j])
                continue;
            set_value = choose_random_number(et, b, i, j, thresh);
            if(set_value != 0)
                free_set_cell(b, i, j, set_value);
        }
    }
    finish_commit(b);
}

void free_possible_entry(PossibleEntry* pe){
    free(pe->valid_nums);
    free(pe->percent);
}

void free_entry_table(EntryTable* et){
    int i, j;
    for(i = 0; i < et->size; i++){
        for(j = 0; j < et->size; j++){
            if(!et->entries[i][j]->value){
                free_possible_entry(et->entries[i][j]);
            }
        }
        free(et->entries[i]);
    }
    free(et->entries);
    free(et->var_arr);
}
