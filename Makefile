CFLAGS	= -g
CC	= gcc
OBJ	= myAllocatorTest1 test1 nextFitTest bestFitTest

all: $(OBJ)

myAllocatorTest1: myAllocator.o myAllocatorTest1.o
	$(CC) $(CFLAGS) -o $@ $^

test1: myAllocator.o malloc.o test1.o
	$(CC) $(CFLAGS) -o $@ $^

nextFitTest: myAllocator.o nextFitTest.o
	$(CC) $(CFLAGS) -o $@ $^

bestFitTest: myAllocator.o bestFitTest.o
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o $(OBJ) 

