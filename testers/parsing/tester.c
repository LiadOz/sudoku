#include <stdio.h>
#include "../../parsing.h"
int main(){
    int i;
    Command cmd;
    while(!feof(stdin)){
        user_input(&cmd);
        printf("name: %s\nargs = %d\n", cmd.name, cmd.arg_length);
        for(i = 0; i < cmd.arg_length; i++){
            printf("%d ", cmd.args[i]);
        }
        printf("\n");
    }
    return 1;
}
