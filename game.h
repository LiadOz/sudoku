#ifndef GAME_H
#define GAME_H

#define SET_CELL_COMMAND "set"
#define HINT_CELL_COMMAND "hint"
#define VALIDATE_COMMAND "validate"
#define RESTART_COMMAND "restart"
#define UNDO_COMMAND "undo"
#define REDO_COMMAND "redo"
#define EXIT_COMMAND "exit"
#define FINISHED 200
#define PRINT_AFTER 100
#define RESTART_AFTER 101
#define INVALID_COMMAND "Error: invalid command\n"
#define SET_READ_ARGS 3
#define HINT_READ_ARGS 2

int game_flow();



#endif
