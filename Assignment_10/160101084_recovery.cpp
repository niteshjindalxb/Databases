#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <algorithm>

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

// Structure of log file
struct LogRecord
{
    char operation;
    int transactionId;
    std::string resource;
    int oldValue;
    int newValue;

    LogRecord(char operation, int t_Id, std::string res, int old_val, int new_val);
    void printRecord();
};

// Structure of log file
class LogFile
{
private:
    std::vector<LogRecord> logFile;
public:
    void addToLogFile(char operation, int transactionId, std::string resource = "", int oldValue = -1, int newValue = -1);
    int getSize();
    LogRecord getRecordAt(int time);
    void printLogFile();
};

LogRecord::LogRecord(char operation, int t_Id, std::string res, int old_val, int new_val) {
    this->operation = operation;
    this->transactionId = t_Id;
    this->resource = res;
    this->oldValue = old_val;
    this->newValue = new_val;
}

void LogRecord::printRecord() {
    switch (this->operation) {
            case 'S' :  std::cout << "< " << this->operation << ", " << this->transactionId << ">\n";
                        break;
            case 'C' :  std::cout << "< " << this->operation << ", " << this->transactionId << ">\n";
                        break;
            case 'W' :  std::cout << "< " << this->operation << ", " << this->transactionId << ", " << this->resource << ", " << this->oldValue << ", " << this->newValue << ">\n";
                        break;
        }
}

void LogFile::addToLogFile(char operation, int transactionId, std::string resource, int oldValue, int newValue) {
    this->logFile.push_back(LogRecord(operation, transactionId, resource, oldValue, newValue));
}

LogRecord LogFile::getRecordAt(int time) {
    return this->logFile.at(time);
}

int LogFile::getSize() {
    return this->logFile.size();
}

void LogFile::printLogFile() {
    std::cout << "------LogFile-------\n";
    for(int i = 0; i < this->getSize(); i++) {
        this->getRecordAt(i).printRecord();
    }    
}

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

            default :   std::cout << "Invalid Input " << operation << " \n";
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
            case 'R' :  this->listResources.read(it->resource);
                        break;

            // Write
            // Write ahead logging
            case 'W' :  this->logFile.addToLogFile(it->operation, it->transactionId, it->resource, this->listResources.read(it->resource), it->value);
                        this->listResources.write(it->resource, it->value);
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
            case 'C' :  // Add undo to logFile
                        // this->logFile.addToLogFile('U', curRecord.transactionId, )
                        this->removeActiveTransaction(curRecord.transactionId);
                        break;
            case 'W' :  this->listResources.write(curRecord.resource, curRecord.newValue);
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

// Print LogFile
void TransactionManagement::printLogFile() {
    this->logFile.printLogFile();
}

// Print Resources
void TransactionManagement::printResources() {
    this->listResources.print();
}

int main(int argc, char const *argv[])
{
    TransactionManagement transactionManager;
    transactionManager.readTransaction();
    transactionManager.performTransaction();
    transactionManager.recoverData();
    transactionManager.printLogFile();
    transactionManager.printResources();
    return 0;
}
