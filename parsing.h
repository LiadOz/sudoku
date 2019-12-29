#ifndef PARSING_H
#define PARSING_H

#define MAX_INPUT_SIZE 1024
void user_input();

/* name - name of the function to execute
 * args - the arguments
 * arg_length - length of the arguments
 * eof - determines if eof has been reached */
typedef struct {
    char* name;
    int args[MAX_INPUT_SIZE-1];
    int arg_length;
    int eof;
} Command;
#endif
