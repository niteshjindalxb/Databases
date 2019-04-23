CXX = g++
CXXFLAGS = -c

all:	build clean

build:	main
	$(CXX) -o run resource.o logFile.o transaction_recovery.o main.o
	

main:	transactionManager
	$(CXX) $(CXXFLAGS) main.cpp

transactionManager:	transaction
	$(CXX) $(CXXFLAGS) transaction_recovery.cpp

transaction:	logFile

logFile:	resource
	$(CXX) $(CXXFLAGS) logFile.cpp

resource:	
	$(CXX) $(CXXFLAGS) resource.cpp
	
clean:
	rm -rf *.o run