#include "transaction_recovery.h"
#include <iostream>
using namespace std;

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
