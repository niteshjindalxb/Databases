CC = g++
CCFLAGS =

all:	extendibleHashing.o main.o
	$(CC) extendibleHashing.o main.o -o main

main.o:	
	$(CC) -c main.cpp

extendibleHashing.o:	
	$(CC) -c extendibleHashing.cpp

clean:
	rm -rf *.o