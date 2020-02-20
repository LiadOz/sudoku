#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "board.h"
#include "possible_table.h"
#include "lp_solver.h"
#include "gurobi_c.h"

/* for test purposes */
#include "printing.h"
#include "solver.h"


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

void find_solution(Board* b){
    GRBenv   *env   = NULL;
    GRBmodel *model = NULL;
    int       error = 0;
    int size;
    char* vtype;
    int       optimstatus;
    double* sol;
    int i;
    EntryTable et;
    init_entry_table(&et, b);

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
    size = et.var_count;
    sol = malloc(size * sizeof(double));
    vtype = malloc(size * sizeof(int));
    memset(vtype, GRB_BINARY, size);
    error = GRBaddvars(model, size, 0, NULL, NULL, NULL, NULL, NULL, NULL, vtype, NULL);
    if (error) {
      printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }
    /* update the model - to integrate new variables */ 
    error = GRBupdatemodel(model);
    if (error) {
      printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }

    add_all_constraints(model, env, &et);
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
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, size, sol);
    if (error) {
      printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
      exit(0);
    }


    printf("\nOptimization complete\n");
    /* solution found */
    if (optimstatus == GRB_OPTIMAL) {
        for(i = 0; i < size; i++){
            if((int)sol[i]){
                printf("%d ", (int)et.var_arr[i]);
            }
        }
    }
    /* no solution found */
    else if (optimstatus == GRB_INF_OR_UNBD) {
    printf("Model is infeasible or unbounded\n");
    }
    /* error or calculation stopped */
    else {
    printf("Optimization was stopped early\n");
    }

    free(vtype);
    free(sol);
    free_entry_table(&et);
    GRBfreemodel(model);
    GRBfreeenv(env);

}
void test(){
    Board b;
    int height = 3;
    int width = 5;
    init_board(&b, height, width);
    createSolution(0, 0, &b);
    set_from_solution(&b, 35);
	printBoard(&b);
    find_solution(&b);
    printf("\n");
    free_board(&b);
}
