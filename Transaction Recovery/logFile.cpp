#include "logFile.h"
#include <iostream>

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