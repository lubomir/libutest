CFLAGS=-Wall -g -Wextra -std=c99 -Isrc
LDFLAGS=-lutest -Lsrc
CC=gcc
STACK_PATH=examples/stack
FILES_PATH=examples/more-files

UTEST=src/libutest.a
STACK_EX=$(STACK_PATH)/stack-test
FILES_EX=$(FILES_PATH)/test-prog

all : $(UTEST) $(STACK_EX) $(FILES_EX)

UTEST_OBJ=src/utest.o
src/utest.o : src/utest.c src/utest.h

STACK_OBJ=$(STACK_PATH)/tests.o $(STACK_PATH)/stack.o
$(STACK_PATH)/stack.o : $(STACK_PATH)/stack.c $(STACK_PATH)/stack.h
$(STACK_PATH)/tests.o : $(STACK_PATH)/tests.c src/utest.h

FILES_OBJ=$(FILES_PATH)/tests1.o $(FILES_PATH)/tests2.o
$(FILES_PATH)/tests1.o : $(FILES_PATH)/tests1.c src/utest.h
$(FILES_PATH)/tests2.o : $(FILES_PATH)/tests2.c src/utest.h

$(UTEST) : $(UTEST_OBJ)
	ar crs $(UTEST) $(UTEST_OBJ)

$(STACK_EX) : $(UTEST) $(STACK_OBJ)
	$(CC) $(CFLAGS) -o $(STACK_EX) $(STACK_OBJ) $(LDFLAGS)

$(FILES_EX) : $(UTEST) $(FILES_OBJ)
	$(CC) $(CFLAGS) -o $(FILES_EX) $(FILES_OBJ) $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<


.PHONY : clean distclean

clean :
	rm -f src/*.o *.o $(STACK_PATH)/*.o $(FILES_PATH)/*.o

distclean : clean
	rm -f $(UTEST) $(STACK_EX) $(FILES_EX)
