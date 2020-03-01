#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

void move_pointer_to_end(Board* b, SetOfMoves** pointer) {
	SetOfMoves* curr;
	curr = b->movePointer;
	while (curr != NULL && curr->next != NULL) {
		curr = curr->next;	
	}
	*pointer = curr;
}

void free_moves(Board* b, SetOfMoves* pointer) {
	SetOfMoves* pointer_to_free;
	while (pointer != NULL && pointer->prev != NULL) {
		pointer_to_free = pointer;
		pointer = pointer->prev;
		free(pointer_to_free->moves);
		free(pointer_to_free);
	}
	b->movePointer = pointer;
}

int restart(Board* b, SetOfMoves* pointer) {
	move_pointer_to_end(b, &pointer);
	free_moves(b, pointer);
	return 1;
}

int undo(Board* b) {
	int i;
	if (b->movePointer != NULL && b->movePointer->prev != NULL) {
		for (i = 0; i < b->movePointer->size; i++) {
			Move* move = &(b->movePointer->moves[i]);
			b->state[move->x][move->y] = move->prevVal;
		}
		b->movePointer = b->movePointer->prev;
		return 1;
	}
	return 0;
}

int redo(Board* b) {
	int i;
	if (b->movePointer != NULL && b->movePointer->next != NULL) {
		b->movePointer = b->movePointer->next;
		for (i = 0; i < b->movePointer->size; i++) {
			Move* move = &(b->movePointer->moves[i]);
			b->state[move->x][move->y] = move->currVal;
		}
		return 1;
	}
	return 0;
}

void init_set_command_move(Board* b, Move* curr_move) {
	SetOfMoves* pointer = malloc(sizeof(SetOfMoves));
	Move* moves_array;
	pointer->size = 1;
	moves_array = malloc(pointer->size * sizeof(Move));
	moves_array[0] = *curr_move;
	pointer->moves = moves_array;
	pointer->prev = b->movePointer;
	if (b->movePointer != NULL) {
		(b->movePointer)->next = pointer;
	}
	b->movePointer = pointer;
}

void print_change(Board* b, char* command) {
	int i;
	int length;
	if (strcmp(command, REDO_COMMAND) == 0) {
		length = b->movePointer->size;
		for (i = 0; i < length; i++) {
			Move* move = &(b->movePointer->moves[i]);
			printf("Cell (%d,%d) has changed from %d to %d",move->x, move->y, move->prevVal, move->currVal);
		}
	}
	else if (strcmp(command, UNDO_COMMAND) == 0) {
		SetOfMoves* moves_before_undo = b->movePointer->next;
		length = moves_before_undo->size;
		for (i = 0; i < length; i++) {
			Move* move = &(moves_before_undo->moves[i]);
			printf("Cell (%d,%d) has changed from %d to %d", move->x, move->y, move->currVal, move->prevVal);
		}
	}
}