#include <unordered_map>
#include <string>

#ifndef RESOURCE_H
#define RESOURCE_H

// Structure of Resouces
class Resource
{
private:
    // <Resource ID, Resource Value>
    std::unordered_map<std::string, int> resource;

public:
    Resource() {}
    int read(std::string resourceId);
    void write(std::string resourceId, int Value);
    void add(std::string resourceId, int Value);
    void print();
};

#endif