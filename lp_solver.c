#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "board.h"
#include "possible_table.h"
#include "lp_solver.h"
#include "gurobi_c.h"

#define ILP 1
#define LP 2
#define RANDOM_UB 15
#define MAX_GENERATE_TRIES 1000

/* creates constraints for cell and inside ind and val
 * returns the length
 * the method assumes the entry is initialized */
int create_cell_constraint(EntryTable* et, int i, int j, int* ind, double* val){
    PossibleEntry entry = et->entries[i][j];
    int k;
    for(k = 0; k < entry.count; k++){
        ind[k] = entry.start_index + k;
        val[k] = 1;
    }
    return k;
}
/* creates constraints for a row inside ind and val
 * size and residuals are returned in the third array */
int create_row_constraint(EntryTable* et, int i, int value, int* ind, double* val){
    int j, k;
    int l = 0;
    PossibleEntry pe;
    for(j = 0; j < et->size; j++){
        pe = et->entries[j][i];
        /* if there is a fixed value it needs to be accounted for */
        if(pe.value == value){
            return 0;
        }
        if(!pe.value){
            for(k = 0; k < pe.count; k++){
                if(et->var_arr[pe.start_index + k] == value){
                    ind[l] = pe.start_index + k;
                    val[l] = 1;
                    l++;
                }
            }
        }
    }
    return l;
}
/* creates constraints for a col inside ind and val
 * size and residuals are returned in the third array */
int create_column_constraint(EntryTable* et, int i, int value, int* ind, double* val){
    int j, k;
    int l = 0;
    PossibleEntry pe;
    for(j = 0; j < et->size; j++){
        pe = et->entries[i][j];
        /* if there is a fixed value it needs to be accounted for */
        if(pe.value == value){
            return 0;
        }
        if(!pe.value){
            for(k = 0; k < pe.count; k++){
                if(et->var_arr[pe.start_index + k] == value){
                    ind[l] = pe.start_index + k;
                    val[l] = 1;
                    l++;
                }
            }
        }
    }
    return l;
}

/* creates constraints for a block inside ind and val
 * size and residuals are returned in the third array */
int create_block_constraint(EntryTable* et, int block_number, int value, int* ind, double* val){
    PossibleEntry pe;
    int block_y = block_number / et->block_height;
    int block_x = block_number - (block_y * et->block_height);
    int start_x = et->block_width * block_x;
    int start_y = et->block_height* block_y;
    int i, j, k, l = 0;
    for(i = start_y; i < et->block_height + start_y; i++){
        for(j = start_x; j < et->block_width + start_x; j++){
            pe = et->entries[i][j];
            if(pe.value == value){
                return 0;
            }
            if(!pe.value){
                for(k = 0; k < pe.count; k++){
                    if(et->var_arr[pe.start_index + k] == value){
                        ind[l] = pe.start_index + k;
                        val[l] = 1;
                        l++;
                    }
                }
            }
        }
    }
    return l;
}

void add_constraint(GRBmodel* model,GRBenv* env, int size, int* ind, double* val, int rhs){
    int error;
    error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, rhs, NULL);
    if (error) {
      printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }
}

void add_all_constraints(GRBmodel* model, GRBenv* env,EntryTable* et){
    int i, j;
    int* ind;
    double* val;
    int size;
    ind = calloc(et->var_count, sizeof(int));
    val = calloc(et->var_count, sizeof(double));
    for(i = 0; i < et->size; i++){
        for(j = 0; j < et->size; j++){
            if(!et->entries[i][j].value){
                /* constraint for each cell */
                size = create_cell_constraint(et, i, j, ind, val);
                add_constraint(model, env, size, ind, val, 1);
            }
        }
        /* go over each number */
        for(j = 1; j < et->size+1; j++){
            size = create_row_constraint(et, i, j, ind, val);
            if(size)
                add_constraint(model, env, size, ind, val, 1);
            size = create_column_constraint(et, i, j, ind, val);
            if(size)
                add_constraint(model, env, size, ind, val, 1);
            size = create_block_constraint(et, i, j, ind, val);
            if(size)
                add_constraint(model, env, size, ind, val, 1);
        }
    }
    free(ind);
    free(val);
}

/* 
 * Uses Gurobi to find solution to board,
 * uses either ILP or LP modes.
 * sol must be supplied.
 */
int find_solution(EntryTable* et, double* sol, int mode){
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    char* vtype;
    double* obj;
    int i, size, optimstatus, error = 0;
    int ret_val = NO_SOLUTION_FOUND;
    double objval;

    /* allocating variables */
    size = et->var_count;
    vtype = malloc(size * sizeof(int));
    if (mode == LP){
        memset(vtype, GRB_CONTINUOUS, size);
        obj = calloc(size, sizeof(double));
        for(i = 0; i < size; i++){
            obj[i] = rand() % RANDOM_UB;
        }
    }
    else if (mode == ILP){
        memset(vtype, GRB_BINARY, size);
        obj = NULL;
    }

    error = GRBloadenv(&env, "mip1.log");
    if (error) {
        printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
        exit(0);
    }
    /* hides information */
    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
        exit(0);
    }
    error = GRBnewmodel(env, &model, "test", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
      printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }

    error = GRBaddvars(model, size, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
    if (error) {
      printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }
    if(mode == LP){
        error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
        if (error) {
          printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
          exit(0);
        }
    }
    /* update the model - to integrate new variables */ 
    error = GRBupdatemodel(model);
    if (error) {
      printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }

    add_all_constraints(model, env, et);
    /* Optimize model - need to call this before calculation */
    error = GRBoptimize(model);
    if (error) {
      printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }

    /* Write model to 'mip1.lp' - this is not necessary but very helpful */
    error = GRBwrite(model, "mip1.lp");
    if (error) {
      printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
      printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }
    if(mode == LP){
        error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
        if (error) {
          printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
          exit(0);
        }
    }

    /* solution found */
    if (optimstatus == GRB_OPTIMAL) {
        error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, size, sol);
        if (error) {
          printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
          exit(0);
        }
        ret_val = SOLUTION_FOUND;
        /* for debug use */
        /*
        if(mode == ILP){
            for(i = 0; i < size; i++){
                if((int)sol[i]){
                    printf("%d ", (int)et->var_arr[i]);
                }
            }
            printf("\n");
        }
        if (mode == LP){
            for(i = 0; i < size; i++){
                printf("%1.1f ", sol[i]);
            }
            printf("\n%f\n", objval);
        }
        */
    }
    /* no solution found */
    else {
        ret_val = NO_SOLUTION_FOUND;
    }
    /* error or calculation stopped */

    free(vtype);
    free(obj);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return ret_val;
}

/* Checks for solution to the board using LP
 * if no solution exits NO_SOLUTION_FOUND is returned 
 * otherwise SOLUTION_FOUND is returned */

int validate_board(Board* b){
    EntryTable et;
    int ret_val;
    double* sol = NULL;
    init_entry_table(&et, b);
    sol = malloc(et.var_count * sizeof(double));

    ret_val = find_solution(&et, sol, ILP);

    free_entry_table(&et);
    free(sol);
    return ret_val;
}

/* gives the answer to a cell using ILP
 * if no solution exits NO_SOLUTION_FOUND is returned */
int ILP_hint(Board* b, int i, int j){
    EntryTable et;
    PossibleEntry pt;
    int ret_val;
    double* sol = NULL;
    int k;
    init_entry_table(&et, b);
    sol = malloc(et.var_count * sizeof(double));

    ret_val = find_solution(&et, sol, ILP);
    if(ret_val == SOLUTION_FOUND){
        pt = et.entries[i][j];
        for(k = pt.start_index; k < pt.end_index - 1; k++){
            if(sol[k] == 1.0)
                ret_val = et.var_arr[k];
        }
    }
    free_entry_table(&et);
    free(sol);
    return ret_val;
}

/* gives a guess to the board using LP 
 * if no solution exits NO_SOLUTION_FOUND is returned */
int guess_board(Board* b, double thresh){
    EntryTable et;
    double* sol = NULL;
    int ret_val;
    init_entry_table(&et, b);
    sol = malloc(et.var_count * sizeof(double));

    ret_val = find_solution(&et, sol, LP);
    insert_solution(&et, sol);
    fill_with_thresh(&et, b, thresh);

    free(sol);
    free_entry_table(&et);
    return ret_val;
}


/* Prints scores for available values 
 * if no solution exits NO_SOLUTION_FOUND is returned */
int guess_hint(Board* b, int i, int j){
    EntryTable et;
    PossibleEntry pt;
    double* sol = NULL;
    int ret_val;
    int k;
    init_entry_table(&et, b);
    sol = malloc(et.var_count * sizeof(double));

    ret_val = find_solution(&et, sol, LP);
    if(ret_val == SOLUTION_FOUND){
        pt = et.entries[i][j];
        for(k = pt.start_index; k < pt.end_index-1; k++){
            if(sol[k] >= 0.01)
                printf("%d : %1.2f%% \n", et.var_arr[k], sol[k]);
        }
    }

    free(sol);
    free_entry_table(&et);
    return ret_val;
}

/* fills ILP solution into board */
void fill_solution(Board* b, EntryTable* et, double* sol){
    int i, j, k;
    PossibleEntry pt;
    for (i = 0; i < b->size; i++){
        for(j = 0; j < b->size; j++){
            pt = et->entries[i][j];
            if(!pt.value){
                for(k = pt.start_index; k < pt.end_index - 1; k++){
                    if(sol[k] == 1.0)
                        b->state[i][j] = et->var_arr[k];
                }
            }
            b->fixed[i][j] = 1;
        }
    }
}

/* generates a board using ILP first it generate x cells in random
 * then finds a solution and removes y cells from it
 * if generation fails MAX_GENERATE_TRIES times it returns NO_SOLUTION_FOUND
 * otherwise SOLUTION_FOUND is returned */
int generate_using_ILP(Board* b, int x, int y){
    EntryTable et;
    int tries = 0;
    double* sol = NULL;
    while (tries < MAX_GENERATE_TRIES){
        if(generate_random_cells(b, x) == BOARD_SETTING_ERROR){
            tries++;
            continue;
        }
        init_entry_table(&et, b);
        sol = malloc(et.var_count * sizeof(double));
        if(find_solution(&et, sol, ILP) == SOLUTION_FOUND){
            /* create the solution */
            fill_solution(b, &et, sol);
            generate_from_solution(b, y);
            free(sol);
            return SOLUTION_FOUND;
        }
        reset_board_state(b);
        tries++;
        free_entry_table(&et);
        free(sol);
    }
    return NO_SOLUTION_FOUND;
}
