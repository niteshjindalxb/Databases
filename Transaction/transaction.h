#ifndef TRANSACTION_H
#define TRANSACTION_H

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

    // Check recoverability
    bool isRecoverable();

    // Check cascadeless Rollback
    bool isCascadeLessRollBack();
        
    // Extract all commit times from transactions
    void getCommit();
};


#endif