#include "resource.h"

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