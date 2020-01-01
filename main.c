#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int main(int argc, char *argv[]){
    if (argc > 1){
        srand(atoi(argv[1]));
    }
    else{
        srand(5);
    }
    game_flow();
    return 1;
}
