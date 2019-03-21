CC = g++
CCFLAGS =

all:	build clean

build:	extendibleHashing.o main.o
	$(CC) -g extendibleHashing.o main.o -o main

main.o:	
	$(CC) -c main.cpp

extendibleHashing.o:	
	$(CC) -c extendibleHashing.cpp

clean:
	rm -rf *.o