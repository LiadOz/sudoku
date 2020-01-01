#include <stdlib.h>
#include <string.h>
#include "game.h"

int main(int argc, char *argv[]){
    if (argc > 0){
        srand(atoi(argv[0]));
    }
    else{
        srand(5);
    }
    game_flow();
    return 1;
}
