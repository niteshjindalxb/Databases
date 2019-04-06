CC = g++
CCFLAGS =

BTree:	build_BTree
	rm -rf *.o *.out


build_BTree:	BTree.o main.o
	$(CC) -g BTree.o main.o -o main
	

main.o:	
	$(CC) -c main.cpp


BTree.o:	
	$(CC) -c BTree.cpp


clean:
	rm -rf *.o *.out main