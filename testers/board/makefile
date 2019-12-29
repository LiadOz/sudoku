CC = gcc
OBJS = tester.o board.o printing.o
EXEC = tester
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ 
tester.o: tester.c
	$(CC) $(COMP_FLAG) -c $*.c
board.o: ../../board.c ../../board.h
	$(CC) $(COMP_FLAG) -c ../../$*.c
printing.o: ../../printing.c ../../printing.h
	$(CC) $(COMP_FLAG) -c ../../$*.c
clean:
	rm -f $(OBJS) $(EXEC)
