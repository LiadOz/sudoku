CC = gcc
OBJS = main.o game.o board.o parsing.o solver.o printing.o possible_table.o lp_solver.o wr_file.o moves.o backtrack.o util.o
EXEC = sudoku-console
COMP_FLAG = -ansi -g -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@
main.o: main.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
board.o: board.c board.h
	$(CC) $(COMP_FLAG) -c $*.c
parsing.o: parsing.c parsing.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.c solver.h
	$(CC) $(COMP_FLAG) -c $*.c
printing.o: printing.c printing.h
	$(CC) $(COMP_FLAG) -c $*.c
lp_solver.o: lp_solver.c lp_solver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
possible_table.o: possible_table.c possible_table.h
	$(CC) $(COMP_FLAG) -c $*.c
wr_file.o: wr_file.c wr_file.h
	$(CC) $(COMP_FLAG) -c $*.c
moves.o: moves.c moves.h
	$(CC) $(COMP_FLAG) -c $*.c
backtrack.o: backtrack.c backtrack.h
	$(CC) $(COMP_FLAG) -c $*.c
util.o: util.c util.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
all: $(EXEC)
