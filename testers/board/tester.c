#include <stdio.h>
#include "../../board.h"
#include "../../printing.h"

#define CASES 6
#define SIZES 2

int main(){
    int i, j, val;
    int t_case;
    int mult;
    Board b;
    int test_cases[CASES][SIZES];
    test_cases[0][0] = 1;
    test_cases[0][1] = 8;
    test_cases[1][0] = 3;
    test_cases[1][1] = 2;
    test_cases[2][0] = 7;
    test_cases[2][1] = 1;
    test_cases[3][0] = 3;
    test_cases[3][1] = 3;
    test_cases[4][0] = 9;
    test_cases[4][1] = 1;
    test_cases[5][0] = 4;
    test_cases[5][1] = 2;
    for(t_case = 0; t_case < CASES; t_case++){
        init_board(&b, test_cases[t_case][0], test_cases[t_case][1]);
        mult = test_cases[t_case][0] * test_cases[t_case][1];
        for(i = 0; i < mult; i++){
            for(j = 0; j < mult; j++){
                for(val = 1; val <= mult; val++){
                    set_cell(&b, i, j, val);
                }
            }
        }
        /* you could print to get a better look at the result but the final test should be without */
        /*printBoard(&b); */
    }
    free_board(&b);
    return 1;
}
