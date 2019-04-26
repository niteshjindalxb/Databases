#include <vector>
#include <string>

#ifndef LOGFILE_H
#define LOGFILE_H

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

#endif