#include "transaction_recovery.h"
#include <iostream>
#include <algorithm>

void TransactionManagement::readTransaction() {
    int transactionId;
    char operation;
    std::string resourceId;
    int value;

    int numResources;

    std::cin >> numResources;
    for(int i = 0; i < numResources; i++) {
        std::cin >> resourceId;
        std::cin >> value;
        this->listResources.add(resourceId, value);
    }

    while ( std::cin >> operation ) {
        
        transactionId = -1;
        operation = '-';
        resourceId = '-';
        value = -1;

        switch (operation) {
            
            // Start
            case 'S' :  std::cin >> transactionId;
                        this->transaction.push_back(Transaction(transactionId, resourceId, operation, value));
                        break;

            // Commit
            case 'C' :  std::cin >> transactionId;
                        this->transaction.push_back(Transaction(transactionId, resourceId, operation, value));
                        break;

            // Read
            case 'R' :  std::cin >> transactionId;
                        std::cin >> resourceId;
                        this->transaction.push_back(Transaction(transactionId, resourceId, operation, value));
                        break;

            // Write
            case 'W' :  std::cin >> transactionId;
                        std::cin >> resourceId;
                        std::cin >> value;
                        this->transaction.push_back(Transaction(transactionId, resourceId, operation, value));
                        break;

            default :   std::cout << "Invalid Input\n";
                        exit(EXIT_FAILURE);
                        break;
        }
    }
}

void TransactionManagement::addActiveTransaction(int transactionId) {
    this->activeTransaction.insert(transactionId);
}

void TransactionManagement::removeActiveTransaction(int transactionId) {
    this->activeTransaction.erase(transactionId);
}

void TransactionManagement::performTransaction() {
    for (auto it = this->transaction.begin(); it != this->transaction.end(); it++) {
        char operation = it->operation;

        switch (operation) {
            
            // Start
            case 'S' :  this->logFile.addToLogFile(it->operation, it->transactionId);
                        this->addActiveTransaction(it->transactionId);
                        break;

            // Commit
            case 'C' :  this->logFile.addToLogFile(it->operation, it->transactionId);
                        this->removeActiveTransaction(it->transactionId);
                        break;

            // Read
            case 'R' :  int value = this->listResources.read(it->resource);
                        break;

            // Write
            // Write ahead logging
            case 'W' :  this->logFile.addToLogFile(it->operation, it->transactionId, it->resource, this->listResources.read(it->resource), it->value);
                        this->listResources.write(it->resource, it->value);
                        break;

            default :   std::cout << "Invalid Operation\n";
                        break;
        }
    }
}

// Recover data using logFile
void TransactionManagement::recoverData() {
    // Assuming active list of transaction also gets deleted, since systems got crashed
    this->activeTransaction.clear();

    // Re-do operation from logFile : Top-down approach
    int sizeOfLogFile = this->logFile.getSize();

    for(int i = 0; i < sizeOfLogFile; i++) {
        LogRecord curRecord = this->logFile.getRecordAt(i);
        
        switch (curRecord.operation) {
            case 'S' :  this->addActiveTransaction(curRecord.transactionId);
                        break;
            case 'C' :  this->removeActiveTransaction(curRecord.transactionId);
                        break;
            case 'W' :  this->listResources.write(curRecord.resource, curRecord.newValue);
                        break;
            default :   std::cout << "Invalid Operation in LogFile\n";
                        break;
        }
    }
    

    // Un-Do all operations of active transactions that got crashed : Bottom-up approach
    for(int i = sizeOfLogFile-1; i >= 0; i--) {
        if (this->activeTransaction.empty())
            break;
            
        LogRecord curRecord = this->logFile.getRecordAt(i);

        if (this->activeTransaction.find(curRecord.transactionId) != this->activeTransaction.end()) {
            if (curRecord.operation == 'S')
                this->removeActiveTransaction(curRecord.transactionId);
            else
                this->listResources.write(curRecord.resource, curRecord.oldValue);
        }
    }
}