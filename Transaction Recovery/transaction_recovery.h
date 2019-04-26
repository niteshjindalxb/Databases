#include <vector>
#include <set>
#include <string>
#include "resource.h"
#include "transaction.h"
#include "logFile.h"

#ifndef TRANSACTION_RECOVERY_H
#define TRANSACTION_RECOVERY_H

// Structure of transaction management system
class TransactionManagement
{
private:
    // Contains info about all the resources
    Resource listResources;

    // List of active transactions
    std::set<int> activeTransaction;

    // Contains transactions with <id, operation, resourceId, newValue>
    std::vector<Transaction> transaction;

    // LogFile for recovery
    LogFile logFile;

public:
    TransactionManagement() {}

    // Read transaction input from stdin
    void readTransaction();

    // Add to list of active transactions
    void addActiveTransaction(int transactionId);

    // Remove from list of active transactions
    void removeActiveTransaction(int transactionId);

    // Perform operations
    void performTransaction();

    // Perform recovery
    void recoverData();

    // Print LogFile
    void printLogFile();

    // Print Resources
    void printResources();
};

#endif