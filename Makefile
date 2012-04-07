UTEST=src/libutest.a
CFLAGS=-Wall -g -Wextra -std=c99 -Isrc
UTEST_OBJ=src/utest.o
CC=gcc

STACK_EX=examples/stack/stack-test
STACK_OBJ=examples/stack/tests.o examples/stack/stack.o

all : $(UTEST) $(STACK_EX)

$(UTEST) : $(UTEST_OBJ) src/utest.h
	ar crs $(UTEST) $(UTEST_OBJ)

$(STACK_EX) : $(LIB) $(STACK_OBJ)
	$(CC) $(CFLAGS) -o $(STACK_EX) $(STACK_OBJ) -lutest -Lsrc

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY : clean distclean

clean :
	rm -f src/*.o *.o examples/stack/*.o

distclean : clean
	rm -f $(UTEST) $(STACK_EX)
