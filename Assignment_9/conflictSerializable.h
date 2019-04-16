#ifndef CONFLICT_SERIAL_H
#define CONFLICT_SERIAL_H

#include <vector>
#include <unordered_map>
#include <utility>

class Transaction
{

    
private:
    // Contains transactions with <id, operation, resourceId>
    std::vector<std::pair<int, std::pair<char, int> > > transaction;

    // Keep track of commit time <transactionId, list of commit times>
    std::unordered_map<int, std::vector<int> > commit;
    
    
public:
    // Read transaction input from stdin
    void read();

    // Is serializable?
    void isSerializable();

    bool isCycle(std::vector< std::pair< std::pair<int, bool>, std::vector<int> > > graph);

    bool checkCycle(std::unordered_map< int, std::pair<int, std::vector<int> > > myGraph, int current);

};


#endif