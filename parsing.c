#include <stdio.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024
#define SET_CELL_COMMAND "set"
#define HINT_CELL_COMMAND "hint"
#define VALIDATE_COMMAND "validate"
#define SET_READ_ARGS 3
#define HINT_READ_ARGS 2


int main(){
    char input[MAX_INPUT_SIZE];
    char* operation;
    int i;
    fgets(input, MAX_INPUT_SIZE, stdin);  
    printf("%s\n", input);
    for(i = 0; i < MAX_INPUT_SIZE; i++){
        printf("%c", input[i]);
    }
    operation = strtok(input, " ");
    printf("\n%s\n", operation);
}



void set(int x, int y, int z){
    printf("setting cell (%d,%d) to %d (placeholder)", x, y, z);
}
void hint(int x, int y){
    printf("hint for cell (%d,%d) is 0 (placeholder)", x, y);
}
void validate(){
    printf("validating ... (placeholder)");
}
