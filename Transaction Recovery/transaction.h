#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

struct Transaction
{
    int transactionId;
    std::string resource;
    char operation;
    int value;

    Transaction(int transactionId, std::string resource, char operation, int value) {
        this->transactionId = transactionId;
        this->resource = resource;
        this->operation = operation;
        this->value = value;
    }
};


#endif