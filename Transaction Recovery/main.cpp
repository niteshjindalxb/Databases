#include "transaction_recovery.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    TransactionManagement transactionManager;
    transactionManager.readTransaction();
    return 0;
}
