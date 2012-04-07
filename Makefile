PROGRAM=tester
LIB=src/libutest.a
CFLAGS=-Wall -g -Wextra -std=c99
LIBOBJ=src/utest.c
OBJ=tests.c stack.c more-tests.c
CC=gcc

$(PROGRAM) : $(LIB) $(OBJ) stack.h
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJ) -Isrc -lutest -Lsrc

$(LIB) : $(LIBOBJ) src/utest.h
	$(CC) $(CFLAGS) src/utest.c -c -o src/utest.o
	ar rs $(LIB) src/utest.o

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY : clean distclean

clean :
	rm -f src/*.o
	rm -f *.o

distclean : clean
	rm -f $(LIB)
	rm -f $(PROGRAM)

