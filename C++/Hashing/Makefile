CC = g++
CCFLAGS =

extendibleHashing:	build_extendible
	rm -rf *.o


linearHashing:	build_linearHashing
	rm -rf *.o


build_linearHashing:	linearHashing.o main.o
	$(CC) -g linearHashing.o main.o -o main
	

build_extendible:	extendibleHashing.o main.o
	$(CC) -g extendibleHashing.o main.o -o main
	

main.o:	
	$(CC) -c main.cpp


extendibleHashing.o:	
	$(CC) -c extendibleHashing.cpp


linearHashing.o:	
	$(CC) -c linearHashing.cpp


clean:
	rm -rf *.o