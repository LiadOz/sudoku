#include <stdlib.h>
#include "util.h"

/* Returns NOT_INT if is not int
 * if it is int it returns the number */
int check_if_int(char* s, int* flag){
    int num = atoi(s);
    if(num == 0 && s[0] != '0')
        *flag = NOT_INT;
        return 0;
    return num;
}
