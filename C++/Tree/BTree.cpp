#include <iostream>
#include <algorithm>
#include <queue>
#include "BTree.h"
using namespace std;

/*
 * Member declarations of a BTree
 * */
BTree::BTree(int numDataPointers) {
    this->numDataPointers = numDataPointers;
    this->root = NULL;
}

// Print BTree in BFS manner
void BTree::printBTree() {
    class Node* temp = this->root;
    queue<Node*> s;
    s.push(this->root);

    while(!s.empty()) {
        temp = s.front();
        s.pop();
        // while (temp != NULL) {
            temp->printNode();
            // Go through all its child
            // Push all its child into stack
            for(int i = 0; i < temp->get_child_size(); i++) {
                s.push(temp->get_child_at_index(i));
            }
        // }
    }
}

// By default it will do pro_active_splitting
bool BTree::insert_element(int key, bool pro_active_split) {
    if (this->root == NULL) {
        this->root = new Node(this->numDataPointers);
    }

    if (pro_active_split) {
        
        // Search till the leaf and split everything comes in between that is full
        // Special case : Splitting root, if it is full
        if (this->root->is_Leaf()) {
            if (this->root->is_full()) {
                this->root->insert_element(key);
                this->root->splitNode();
                this->root = this->root->get_parent();
            }
            else {
                this->root->insert_element(key);
            }
            return NO_ERROR;
        }
        
        if (this->root->is_full()) {

            this->root->splitNode();
            this->root = this->root->get_parent();
        }

        class Node* temp = this->root;
        // Search till the leaf and split everything comes in between that is full
        while (temp->is_Leaf() == false) {
        
            if (temp->is_full()) {
                temp->splitNode();
                temp = temp->get_parent();
            }

            // Search for the appropriate child
            int position = temp->lower_bound_on_child(key);

            // Get next node to search on
            temp = temp->get_child_at_index(position);
        }

        // We got the leaf
        if (temp->is_Leaf()) {

            // If leaf is full
            if (temp->is_full()) {
                temp->insert_element(key);
                temp->splitNode();
            }
            else {
                temp->insert_element(key);
            }
        }
    }
    else {
        // Find the leaf
        Node *temp = this->root;
        while (temp->is_Leaf() == false) {
            temp = temp->get_child_at_index(temp->lower_bound_on_child(key));
        }
        if (temp->is_full()) {
            temp->insert_element(key);
            while (temp->is_overflow()) {
                temp->splitNode();
                if (temp == this->root)     this->root = temp->get_parent();
                temp = temp->get_parent();
            }
        }
        else
            temp->insert_element(key);
    }
    return NO_ERROR;
}

// Search element in the BTree
// Returns Node if found, else returns NULL
Node* BTree::search_element(int key) {
    if (this->root == NULL)     return NULL;
    Node *temp = this->root;
    while(!temp->is_Leaf()) {
        // Search element in current node
        if(temp->search_element(key))
            return temp;
        temp = temp->get_child_at_index(temp->lower_bound_on_child(key));
    }
    if (temp->search_element(key))
        return temp;
    else
        return NULL;
}

/*
 * Member declarations of a Node
 * */
Node::Node(int numDataPointers, int depth, int id) {
    this->max_size = numDataPointers;
    this->min_size = (numDataPointers + 1)/2;
    this->depth = depth;
    this->parent = NULL;
    this->id = id;
}

int Node::get_size() {
    return this->key.size();
}

int Node::get_child_size() {
    return this->child.size();
}

int Node::get_depth() {
    return this->depth;
}

int Node::get_numDataPointers() {
    return this->max_size;
}

bool Node::is_full() {
    return this->get_size() == this->max_size;
}

bool Node::is_overflow() {
    return this->get_size() > this->max_size;
}

bool Node::is_Leaf() {
    return this->get_depth() == 0;
}

void Node::increase_depth() {
    this->depth++;
}

int Node::lower_bound_on_child (int key) {
    vector<int>::iterator it = lower_bound(this->key.begin(), this->key.end(), key);
    return it - this->key.begin();
}

class Node* Node::get_new_node(int t) {
    return new Node(t);
}

class Node* Node::get_child_at_index(int index) {
    return this->child[index];
}

int Node::get_key_at_index(int index) {
    return this->key[index];
}

class Node* Node::get_parent() {
    return this->parent;
}

// Returns position where key is inserted
int Node::insert_element (int key) {
    // Insert key into list of keys in sorted order
    // Get the required location
    vector<int>::iterator it = lower_bound(this->key.begin(), this->key.end(), key);
    int distance = it - this->key.begin();
    // Insert into the vector
    this->key.insert(it, key);

    return distance;
}

// Search element in this node
// Returns true, if found
bool Node::search_element(int key) {
    return binary_search(this->key.begin(), this->key.end(), key);
}

// Split the current node and send the middle element to the parent node
void Node::splitNode() {
    // Get the parent node
    class Node* parent = this->parent;

    /*********** Creating a right node **************/

    // Create a node for right node
    class Node *new_node = this->get_new_node(this->get_numDataPointers());
    // Get keys for the right node
    new_node->key.insert(new_node->key.end(), this->key.begin()+this->key.size()/2 + 1, this->key.end());
    // Get child for right node
    new_node->child.insert(new_node->child.end(), this->child.begin()+this->child.size()/2, this->child.end());
    // Set parent of the node
    new_node->parent = parent;
    // Set depth of the new node
    new_node->depth = this->depth;
    // Set ID of node
    new_node->id = this->id + 1;
    
    /*********** Creating a left node **************/

    // key needed to send to parent
    int key = this->key[this->key.size()/2];

    // Keep this node as a left node, so delete right_keys and child from the current node
    this->key.erase(this->key.begin()+this->key.size()/2, this->key.end());
    this->child.erase(this->child.begin()+this->child.size()/2, this->child.end());

    /*********** Send the middle key to the parent **************/

    // If parent does not exists (Splitting the root case)
    if (parent == NULL) {

        // Create a new node for root
        parent = this->get_new_node(this->get_numDataPointers());
        // Set left child of parent[0] = this
        parent->child.push_back(this);
        // set id of parent
        parent->id = this->id + 2;
        parent->depth = this->depth + 1;
    }
    
    int position = parent->insert_element(key);

    // Correctly inserted
    if (position != -1) {
        // Left child is already set at its required child position
        // Insert right child at parent[position+1]
        parent->child.insert(parent->child.begin() + position + 1, new_node);
    }
    else {
        cout << "Error splitting node ...\n";
        exit(-1);
    }

    // Set the parent of both the child
    this->parent = parent;
    new_node->parent = parent;
}


// Print Node ID, Node keys and ID of children
void Node::printNode() {
    cout << "ID : " << this->id << " Depth : " << this->get_depth() << "\tElements : ";
    for(int i = 0; i < this->get_size(); i++) {
        cout << this->key[i] << " ";
    }
    cout << endl;
    cout << "Child ID : ";
    for(int i = 0; i < this->child.size(); i++) {
        cout << this->child[i]->id << " ";
    }
    cout << endl;
}


/*
 *  To Do :
 *          Handle depth
 * */

/* Main for testing purpose */
// int main(int argc, char const *argv[])
// {
//     class BTree root;
//     root.insert_element(1, false);
//     root.insert_element(2, false);
//     root.insert_element(3, false);
//     root.insert_element(23, false);
//     root.insert_element(22, false);
//     root.printBTree();
//     cout << endl;
//     root.insert_element(34, false);
//     root.insert_element(263, false);
//     root.insert_element(-2, false);
//     root.insert_element(0, false);
//     root.insert_element(-23, false);
//     root.printBTree();

//     // Search
//     if (root.search_element(-23) != NULL)
//         cout << "FOUND\n";
//     return 0;
// }
