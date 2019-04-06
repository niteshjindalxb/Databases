CXX = g++ -std=c++11
CXXFLAGS= -std=c++11

all:	build	clean
	./run

debug:	build	clean
	gdb ./run

build:	 build_mainMem build_Disk build_extSort build_main
	$(CXX) -g mainMemory.o disk.o externalMergeSort.o main.o -o run

build_Disk:
	$(CXX) -c disk.cpp

build_mainMem:
	$(CXX) -c mainMemory.cpp

build_extSort:
	$(CXX) -c externalMergeSort.cpp

build_main:
	$(CXX) -c main.cpp

clean:
	rm -rf *.o *.out
