CC = gcc
OBJS = main.o board.o parsing.o
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
main.o: main.c board.h parsing.h
	$(CC) $(COMP_FLAG) -c $*.c
board.o: board.c board.h
	$(CC) $(COMP_FLAG) -c $*.c
parsing.o: parsing.c parsing.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
