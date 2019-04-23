#include "logFile.h"

LogRecord::LogRecord(char operation, int t_Id, std::string res, int old_val, int new_val) {
    this->operation = operation;
    this->transactionId = t_Id;
    this->resource = res;
    this->oldValue = old_val;
    this->newValue = new_val;
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