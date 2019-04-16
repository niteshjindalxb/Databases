#include "conflictSerializable.h"
#include <iostream>
#include <stack>

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

bool Transaction::checkCycle(std::unordered_map< int, std::pair<int, std::vector<int> > > myGraph, int current) {
    if (current == -1) {
        for(auto it = myGraph.begin(); it != myGraph.end(); it++) {
            if (it->second.first == 0) {
                myGraph[it->first].first = 5;
                if (this->checkCycle(myGraph, it->first))
                    return true;
                it->second.first = 10;
            }
        }
        
    }
    else {
        for(auto neigh = myGraph[current].second.begin(); neigh != myGraph[current].second.end(); neigh++) {
            if (myGraph[*neigh].first == 5)
                return true;
            else if (myGraph[*neigh].first == 10)
                continue;
            else {
                if (this->checkCycle(myGraph, *neigh))
                    return true;
            }
        }
        myGraph[current].first = 10;
    }
    return false;
}

bool Transaction::isCycle(std::vector< std::pair< std::pair<int, bool>, std::vector<int> > > graph) {


    std::unordered_map< int, std::pair<int, std::vector<int> > > myGraph;
    for(auto graph_it = graph.begin(); graph_it != graph.end(); graph_it++) {
        std::vector<int> neigh;
        for(auto it = graph_it->second.begin(); it != graph_it->second.end() ; it++) {
            neigh.push_back(*it);
        }
        myGraph[graph_it->first.first].second = neigh;
        myGraph[graph_it->first.first].first = 0;
    }

    return this->checkCycle(myGraph, -1);
}

void Transaction::isSerializable() {

    // Make a graph with node consist of transaction IDs
    std::vector< std::pair< std::pair<int, bool>, std::vector<int> > > graph;

    // Iterate over transactions
    std::vector<std::pair<int, std::pair<char, int> > >::iterator pivot = this->transaction.begin();


    // Iterate over each transaction after pivot and add edges from pivot to it, if the transaction is conflicting
    while (pivot != this->transaction.end()) {

        // std::cout << "Position : " << pivot - this->transaction.begin() << "\n";

        int pivotResource = pivot->second.second;
        char pivotOperation = pivot->second.first;
        int pivotId = pivot->first;

        bool present = false;
        for(auto graph_it = graph.begin(); graph_it != graph.end(); graph_it++) {
            if (graph_it->first.first == pivotId) {
                present = true;
                break;
            }
        }
        if (!present) {
            // Add node to graph
            std::pair< std::pair<int, bool>, std::vector<int> > node;
            std::vector<int> neighBour;
            node.first.first = pivotId;
            node.first.second = false;
            node.second = neighBour;
            graph.push_back(node);
        }
        

        if (pivotOperation == 'C') {
            pivot ++;
            continue;
        }
        
        // Iterate over each transaction after pivot
        for (auto it = pivot; it != this->transaction.end(); it ++) {

            // std::cout << "Inner loop Position : " << it - this->transaction.begin() << "\n";
            if (it->second.first == 'C' or it->first == pivot->first)
                continue;
            else if (it->second.second != pivotResource)
                continue;

            // Check if pivot conflicts with current transaction
            if (pivotOperation == 'W' || it->second.first == 'W') {
                // Add edge from pivot to current node
                for(auto graph_it = graph.begin(); graph_it != graph.end(); graph_it++) {
                    if (graph_it->first.first == pivotId) {
                        bool present = false;
                        for(auto neigh_it = graph_it->second.begin(); neigh_it != graph_it->second.end(); neigh_it++) {
                            if (*neigh_it == it->first) {
                                present = true;
                                break;
                            }
                        }
                        if (!present) {
                            graph_it->second.push_back(it->first);
                        }
                        break;
                    }
                }
            }
        }

        // std::cout << "Outer Loop ...\n";
        pivot ++;
    }

    // Check if there exists any cycle in given graph
    if (this->isCycle(graph)) {
        std::cout << "Conflict serializable:  No\n";
        std::cout << "Commit ordering:  Not applicable\n";
        return;
    }
    else {
        // Check for commit ordering
        // First of all get commit ordering
        // Format: <transactionId, commitTime>
        std::unordered_map<int, int> commitOrder;
        for(auto transaction_it = this->transaction.begin(); transaction_it != this->transaction.end(); transaction_it++) {
            if (transaction_it->second.first == 'C')
                commitOrder[transaction_it->first] = transaction_it-this->transaction.begin();
        }
        
        // Check if such commit ordering is possible via conflict serializable schedule
        for(auto graph_it = graph.begin(); graph_it != graph.end(); graph_it++) {
            
            int pivot_id = graph_it->first.first;
            for(auto neigh_it = graph_it->second.begin(); neigh_it != graph_it->second.end(); neigh_it++) {
                // Commit order should be: neigh_it > graph_it
                int currentId = *neigh_it;
                if (commitOrder[pivot_id > currentId]) {
                    std::cout << "Conflict serializable:  Yes\n";
                    std::cout << "Commit ordering:  No\n";
                    return;
                }
            }
            
        }
        
        std::cout << "Conflict serializable:  Yes\n";
        std::cout << "Commit ordering:  Yes\n";
        return;
    }
}
