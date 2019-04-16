#include "transaction.h"
#include <iostream>

void Transaction::read() {
    int transactionId;
    char operation;
    int resourceId;

    while ( std::cin >> transactionId ) {
        std::cin >> operation;

        switch (operation) {
            case 'C' :  this->transaction.push_back({transactionId, {operation, -1}});
                        break;
            case 'R' :  std::cin >> resourceId;
                        this->transaction.push_back({transactionId, {operation, resourceId}});
                        break;
            case 'W' :  std::cin >> resourceId;
                        this->transaction.push_back({transactionId, {operation, resourceId}});
                        break;
            default :   std::cout << "Invalid Input\n";
                        exit(EXIT_FAILURE);
                        break;
        }
    }
}

void Transaction::getCommit() {
    // Iterate over transactions
    for (auto it = this->transaction.begin(); it != this->transaction.end(); it++) {
        if (it->second.first == 'C') {

            // Get transaction id
            int id = it->first;
            // get time stamp of commit
            int commitTime = it - this->transaction.begin();
            
            if (this->commit.find(id) != this->commit.end()) {
                this->commit[id].push_back(commitTime);
            }
            else {
                std::vector<int> temp;
                temp.push_back(commitTime);
                this->commit[id] = temp;
            }
        }
    }
}


bool Transaction::isRecoverable() {

    // Get commit timings
    this->getCommit();

    // Iterate over transactions
    std::vector<std::pair<int, std::pair<char, int> > >::iterator pivot = this->transaction.begin();

    // Go to first write operation

    while (pivot != this->transaction.end()) {
        // Check if the pivot is read operation then pass
        if (pivot->second.first == 'W')
            break;
        else 
            pivot ++;
    }

    if (pivot != this->transaction.end())
        // Checking if commit time exists
        if (this->commit.find(pivot->first) == this->commit.end()) {
            std::cout << "Commit is not present\nAborting...\n";
            exit(EXIT_FAILURE);
        }

    // Check if each operation "on same resource" after pivot has commit time >= commit time of pivot
    while (pivot != this->transaction.end()) {

        // std::cout << "Position : " << pivot - this->transaction.begin() << "\n";

        int pivotResource = pivot->second.second;
        
        // Iterate over each transaction after pivot
        for (auto it = pivot; it != this->transaction.end(); it ++) {

            // std::cout << "Inner loop Position : " << it - this->transaction.begin() << "\n";

            if (it->second.first == 'C' and it->first == pivot->first)
                break;
            else if (it->second.second != pivotResource)
                continue;


            // Checking if commit time exists
            if (this->commit.find(it->first) != this->commit.end()) {
            // it->commit time should be gt_eq pivot->commit time
                if (this->commit[it->first][0] < this->commit[pivot->first][0]) {

                    std::cout << "Transaction " << it->first << " has commit time less than transaction " << pivot->first << "\n";;
                    
                    return false;
                }
            }
            else {
                std::cout << "Commit is not present\nAborting...\n";
                exit(EXIT_FAILURE);
            }
        }

        pivot ++;
    }
    return true;
}


bool Transaction::isCascadeLessRollBack() {

    // List to maintain all resources with dirty bit = 1 with corresponding transaction id
    // Format : <resource, transaction>
    std::unordered_map<int, int> dirtyResources;

    // Map to maintain which transaction has changed which resources
    // Format: <transactionId, list of resources>
    std::unordered_map<int, std::vector<int> > usedResources;

    // iterate over transaction and check if a resource is being used again while its dirty bit is set 1
    // reset dirty bit of all resources which are accessed by the committing transaction
    for (auto it = this->transaction.begin(); it != this->transaction.end(); it++) {

        // Get transaction id
        int id = it->first;
        int operation = it->second.first;
        int resource = it->second.second;

        // If transaction is committing
        if (operation == 'C') {
            // Free all those resources utilized by the transaction
            for (auto res = usedResources[id].begin(); res != usedResources[id].end(); res++) {
                auto temp = dirtyResources.find(*res);
                dirtyResources.erase(temp);
            }
        }
        // If this resource is already being used
        if (dirtyResources.find(resource) != dirtyResources.end()) {
            // Check if the current transaction is same as the one using it previously
            // If not same, return false
            if (id != dirtyResources[resource]) {

                // Error message
                std::cout << "Transaction " << id << " is using the resource that is un-committed and written by Transaction " << dirtyResources[resource] << "\n";
                
                return false;
            }
        }
        // if resource was not used till now and operation is "write", then set dirty bit = 1 corresponding to current transaction
        else {
            dirtyResources[resource] = id;
            if (usedResources.find(id) != usedResources.end())
                usedResources[id].push_back(resource);
            else {
                std::vector<int> temp;
                temp.push_back(resource);
                usedResources[id] = temp;
            }
        }
    }
    return true;
}