#ifndef PARSING_H
#define PARSING_H

#define MAX_INPUT_SIZE 1024
void user_input();

typedef struct {
    char* name;
    int args[MAX_INPUT_SIZE-1];
    int arg_length;
} Command;

#endif
