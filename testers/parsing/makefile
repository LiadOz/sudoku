CC = gcc
OBJS = tester.o parsing.o
EXEC = tester
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ 
tester.o: tester.c
	$(CC) $(COMP_FLAG) -c $*.c
parsing.o: ../../parsing.c ../../parsing.h
	$(CC) $(COMP_FLAG) -c ../../$*.c
clean:
	rm -f $(OBJS) $(EXEC)
