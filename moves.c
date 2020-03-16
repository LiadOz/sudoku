#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "moves.h"

Move* create_new_move(Board* b, int x, int y, int val) {
	Move* move = (Move*)malloc(sizeof(Move));
	move->x = x;
	move->y = y;
	move->currVal = val;
	move->prevVal = b->state[x][y];
	move->next = NULL;
	return move;
}

void free_next_moves(Board* b) {
	Moves_Bundle* bundle = b->movePointer->next;
	Moves_Bundle* next_bundle;
	Move *move, *next_move;
	while (bundle) {
		move = bundle->head;
		while (move) {
			next_move = move->next;
			free(move);
			move = next_move;
		}
		next_bundle = bundle->next;
		free(bundle);
		bundle = next_bundle;
	}
}


void add_moves_to_board(Board* b, Move* head) {
	Moves_Bundle* bundle = (Moves_Bundle*)malloc(sizeof(Moves_Bundle));
	bundle->head = head;
	if (b->movePointer != NULL) {
		if (b->movePointer->next != NULL) {
			free_next_moves(b);
		}
		b->movePointer->next = bundle;
	}
	bundle->prev = b->movePointer;
	bundle->next = NULL;
	bundle->first = 0;
	b->movePointer = bundle;
}

void move_pointer(Board* b, char* command) {
	if (strcmp(command, REDO_COMMAND) == 0) {
		b->movePointer = b->movePointer->next;
	}
	else if (strcmp(command, UNDO_COMMAND) == 0) {
		b->movePointer = b->movePointer->prev;
	}
}

void print_change(char* command, Move* move) {
	if (strcmp(command, REDO_COMMAND) == 0) {
		printf("Cell (%d,%d) has changed from %d to %d\n", move->x + 1, move->y + 1, move->prevVal, move->currVal);
	}
	else if (strcmp(command, UNDO_COMMAND) == 0) {
		printf("Cell (%d,%d) has changed from %d to %d\n", move->x + 1, move->y + 1, move->currVal, move->prevVal);
	}
}

void exec_moves(Board* b, char* command, int reset) {
	Move* move = b->movePointer->head;
	while (move != NULL) {
		if (strcmp(command, REDO_COMMAND) == 0) {
            /*
			set_cell(b, move->x, move->y, move->currVal, &move, 0);
            */
			set_cell(b, move->x, move->y, move->currVal);
			if (!reset) {
				print_change(REDO_COMMAND, move);
			}
		}
		else if (strcmp(command, UNDO_COMMAND) == 0) {
            /*
			set_cell(b, move->x, move->y, move->prevVal, &move, 0);
            */
			set_cell(b, move->x, move->y, move->prevVal);
			if (!reset) {
				print_change(UNDO_COMMAND, move);
			}
		}
		move = move->next;
	}
}

int undo(Board* b, int reset) {
	if (b->movePointer->first == 1) {
		return 0;
	}
	else {
		exec_moves(b, UNDO_COMMAND, reset);
		move_pointer(b, UNDO_COMMAND);
		return 1;
	}
}

int redo(Board* b, int reset) {
	if (b->movePointer->next == NULL) {
		return 0;
	}
	else {
		move_pointer(b, REDO_COMMAND);
		exec_moves(b, REDO_COMMAND, reset);
		return 1;
	}
}

void reset(Board* b) {
	while (b->movePointer->first != 1) {
		undo(b, 1);
	}
}

void free_all_moves(Board* b) {
	reset(b);
	free_next_moves(b);
	free(b->movePointer);
}

/* creating a container to store the next moves */
void new_commit(Board* b){
    Moves_Bundle* new_commit = malloc(sizeof(Moves_Bundle));
    new_commit->first = 0;
    new_commit->commited = !COMMITED;
    new_commit->head = NULL;
    new_commit->next = NULL;
    new_commit->prev = b->movePointer;

    /* setting move in list and freeing reversed moves */
	if (b->movePointer != NULL) {
		if (b->movePointer->next != NULL) {
			free_next_moves(b);
		}
		b->movePointer->next = new_commit;
    }
	b->movePointer = new_commit;
}

/* adds a move to last uncommited bundle */
void add_move(Board* b, int x, int y, int val){
    Move* move;
    Moves_Bundle* curr_bundle = b->movePointer;
    if(curr_bundle->first == 1 || curr_bundle->commited == COMMITED)
        return;

    /* creating the move */
    move = (Move*)malloc(sizeof(Move));
    move->x = x;
    move->y = y;
    move->currVal = val;
    move->prevVal = b->state[x][y];
    move->next = NULL;

    /* appending move */
    if(curr_bundle->head){
        curr_bundle->tail->next = move;
        curr_bundle->tail = move;
    }
    else{
        curr_bundle->head = move;
        curr_bundle->tail = move;
    }

}

/* closes the commit */
void finish_commit(Board *b){
    b->movePointer->commited = COMMITED;
    /* remove if empty */
    if(b->movePointer->head == NULL){
        b->movePointer = b->movePointer->prev;
        free_next_moves(b);
        b->movePointer->next = NULL;
    }
}
