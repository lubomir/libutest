UTEST=src/libutest.a
CFLAGS=-Wall -g -Wextra -std=c99 -Isrc
LDFLAGS=-lutest -Lsrc
UTEST_OBJ=src/utest.o
CC=gcc

STACK_EX=examples/stack/stack-test
STACK_OBJ=examples/stack/tests.o examples/stack/stack.o

FILES_EX=examples/more-files/test-prog
FILES_OBJ=examples/more-files/tests1.o examples/more-files/tests2.o

all : $(UTEST) $(STACK_EX) $(FILES_EX)

$(UTEST) : $(UTEST_OBJ) src/utest.h
	ar crs $(UTEST) $(UTEST_OBJ)

$(STACK_EX) : $(LIB) $(STACK_OBJ)
	$(CC) $(CFLAGS) -o $(STACK_EX) $(STACK_OBJ) $(LDFLAGS)

$(FILES_EX) : $(LIB) $(FILES_OBJ)
	$(CC) $(CFLAGS) -o $(FILES_EX) $(FILES_OBJ) $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY : clean distclean

clean :
	rm -f src/*.o *.o examples/stack/*.o examples/more-files/*.o

distclean : clean
	rm -f $(UTEST) $(STACK_EX) $(FILES_EX)
