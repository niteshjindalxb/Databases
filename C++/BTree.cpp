#include <iostream>
#include <algorithm>
#include "BTree.h"
using namespace std;

/*
 * Member declarations of a BTree
 * */
BTree::BTree(int t) {
    this->t = t;
    this->root = NULL;
}

class BTree* BTree::createBTree(int t) {
    return new BTree(t);
}

// By default it will do pro_active_splitting
bool BTree::insert_element(int key, bool pro_active_split) {
    
    if (pro_active_split) {
        
        // Search till the leaf and split everything comes in between that is full
        // Special case : Splitting root, if it is full
        if (this->root->is_Leaf() && this->root->is_full()) {
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
    return NO_ERROR;
}

/*
 * Member declarations of a Node
 * */
Node::Node(int t, int depth) {
    this->max_size = 2*t-1;
    this->min_size = t-1;
    this->depth = depth;
    this->parent = NULL;
}

int Node::get_size() {
    return this->key.size();
}

int Node::get_depth() {
    return this->depth;
}

int Node::get_t() {
    return this->min_size + 1;
}

bool Node::is_full() {
    return this->get_size() == this->max_size;
}

bool Node::is_Leaf() {
    return this->get_depth() == 0;
}

void Node::increase_depth() {
    this->depth++;
}

int Node::lower_bound_on_child (int key) {
    vector<class Node*>::iterator it = lower_bound(this->child.begin(), this->child.end(), key);
    return distance(it, this->child.begin());
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

// Returns ERROR if not able to insert, else returns inserted position
int Node::insert_element (int key) {
    if (this->is_full()) {
        // Can't accomodate more keys
        return -1;
    }
    else {
        // Insert key into list of keys in sorted order
        // Get the required location
        vector<int>::iterator it = lower_bound(this->key.begin(), this->key.end(), key);
        // Insert into the vector
        this->key.insert(it, key);

        return it - this->key.begin();
    }
}

// Search element
// Returns true, if found
bool Node::search_element(int key) {
    return binary_search(this->key.begin(), this->key.end(), key);
}

void Node::splitNode() {
    // Get the parent node
    class Node* parent = this->parent;

    /*********** Creating a right node **************/

    // Create a node for right node
    class Node *new_node = this->get_new_node(this->get_t());
    // Get keys for the right node
    new_node->key.insert(new_node->key.end(), this->key.begin()+this->key.size()/2 + 1, this->key.end());
    // Get child for right node
    new_node->child.insert(new_node->child.end(), this->child.begin()+this->child.size()/2, this->child.end());
    // Set parent of the node
    new_node->parent = parent;
    // Set depth of the new node
    new_node->depth = this->depth;
    
    /*********** Creating a left node **************/

    // key needed to send to parent
    int key = this->key[this->key.size()/2];

    // Keep this node as a left node, so delete right_keys and child from the current node
    this->key.erase(this->key.begin()+this->key.size()/2 + 1, this->key.end());
    this->child.erase(this->child.begin()+this->child.size()/2, this->child.end());

    /*********** Send the middle key to the parent **************/

    // If parent does not exists (Splitting the root case)
    if (parent == NULL) {

        // Create a new node for root
        parent = this->get_new_node(this->get_t());
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



/*
 *  To Do :
 *          Handle depth
 * */

/* Main for testing purpose */
int main(int argc, char const *argv[])
{
    class BTree root;
    return 0;
}
