

void move_pointer_to_end(Board* b, SetOfMoves** pointer);
void free_moves(Board* b, SetOfMoves* pointer);
int restart(Board* b, SetOfMoves* pointer);
int undo(Board* b);
int redo(Board* b);
void init_set_command_move(Board* b, Move* curr_move);
void print_change(Board* b, char* command);