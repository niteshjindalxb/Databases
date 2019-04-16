#include "conflictSerializable.h"
#include <iostream>

int main(){

	Transaction obj;

	obj.read();   // to read input file

    // std::cout << "Read transactions\n";


	// if (obj.isRecoverable())  // to check whether schedule is recoverable or not
    //     std::cout << "It's recoverable\n";
    // else
    //     std::cout << "It's not recoverable\n";

	// if (obj.isCascadeLessRollBack())  // to check whether schedule is cascadeless or not 
    //     std::cout << "It's Cascadeless RollBack\n";
    // else
    //     std::cout << "It's not Cascadeless RollBack\n";

    obj.isSerializable();

	return 0;

}