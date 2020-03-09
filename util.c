#include <stdlib.h>
#include "util.h"

/* Returns IS_INT if is not int
 * if it is int it returns the number */
int check_if_int(char* s){
    int num = atoi(s);
    if(num == 0 && s[0] != '0')
        return IS_INT;
    return num;
}
