PROGRAM=tester
CFLAGS=-Wall -g -Wextra -std=c99
OBJ=tests.c stack.c utest.c
CC=gcc

$(PROGRAM) : $(OBJ) utest.h stack.h
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJ) -I.

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY : clean distclean

clean :
	rm -f *.o

distclean : clean
	rm -f $(PROGRAM)

