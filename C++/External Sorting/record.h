#include <vector>

#ifndef RECORD_H
#define RECORD_H

class record
{
private:
    int value;
public:
    int getval() {        return this->value;       }
    void setval(int value) {        this->value = value;       }
    int display() {     std::cout << value << " ";  }
};

#endif