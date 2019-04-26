#include "resource.h"
#include <iostream>

int Resource::read(std::string resourceId) {
    if (this->resource.count(resourceId))
        return this->resource[resourceId];
}

void Resource::write(std::string resourceId, int Value) {
    this->resource[resourceId] = Value;
}

void Resource::add(std::string resourceId, int Value) {
    this->resource[resourceId] = Value;
}

void Resource::print() {
    std::cout << "------Resources------\n";
    for (auto it = this->resource.begin(); it != this->resource.end(); it++) {
        std::cout << it->first << " : " << it->second << "\n";
    }
}