#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>
#include <vector>
using namespace std;

#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#define DEFAULT_NUM_DATA 3
#define DEFAULT_NUM_INDEX 2

#define ERROR false
#define NO_ERROR true

class DataNode* toDataNode(void *);
class IndexNode* toIndexNode(void *);
class Node* toNode(void *);

class Node
{
public:
    int numDataPointers;
    int numIndexPointers;
    int id;
    int max_size;
    int min_size;
    int depth;
    class Node* parent;
    std::vector<int> key;

    Node() {}
    int get_size();
    int get_depth();
    bool is_full();
    bool is_DataNode();
    bool is_IndexNode();
    bool is_overflow();
    bool is_Leaf();
    void increase_depth();
    class Node* get_parent();
    int get_key_at_index(int);
    class Node* get_new_node(int numDataPointers = DEFAULT_NUM_DATA, int numIndexPointers = DEFAULT_NUM_INDEX);
    class IndexNode* get_new_IndexNode(int numDataPointers = DEFAULT_NUM_DATA, int numIndexPointers = DEFAULT_NUM_INDEX);
    int insert_element(int);
    bool search_element(int);
    int get_numIndexPointers();
    int get_numDataPointers();
};

class BPlusTree
{
private:
    int numDataPointers;
    int numIndexPointers;
    int depth;
    void* root;
    
public:
    BPlusTree(int numDataPointers = DEFAULT_NUM_DATA, int numIndexPointers = DEFAULT_NUM_INDEX);
    bool insert_element(int key, bool pro_active_split = false);
    void printBTree();
    Node* search_element(int);
    bool isDataNode();
};

class IndexNode : public Node
{
private:
    std::vector<void*> child;

public:
    IndexNode(int numDataPointers = DEFAULT_NUM_DATA, int numIndexPointers = DEFAULT_NUM_INDEX, int depth = 0, int id = 0);
    int get_child_size();
    int lower_bound_on_child (int);
    void* get_child_at_index(int);
    void set_child_at_index(void*, int position = -1);
    void splitNode();
    void printIndexNode();
    bool isChildData();
    void adjustParentPointer();
    void adjustID();
};

class DataNode : public Node
{
private:
    // Key "value" pair
    std::vector<void*> value;
    DataNode* left;
    DataNode* right;

public:
    DataNode(int numDataPointers = DEFAULT_NUM_DATA, int numIndexPointers = DEFAULT_NUM_INDEX, int depth = 0, int id = 0);
    class DataNode* get_new_DataNode(int numDataPointers = DEFAULT_NUM_DATA, int numIndexPointers = DEFAULT_NUM_INDEX);
    int get_separator(int, int);
    DataNode* splitNode(int factor);
    void printDataNode();
};

#endif // end of BPLUSTREE_H



class DataNode* toDataNode(void *x) {
    return (DataNode*)x;
}

class IndexNode* toIndexNode(void *x) {
    return (IndexNode*)x;
}

class Node* toNode(void *x) {
    return (Node*)x;
}


/* *******************************************************************************
 * Member declarations of a BPlusTree
 * *******************************************************************************/
BPlusTree::BPlusTree(int numDataPointers, int numIndexPointers) {
    this->numDataPointers = numDataPointers;
    this->numIndexPointers = numIndexPointers;
    this->root = NULL;
}

// Print BPlusTree in BFS manner
void BPlusTree::printBTree() {
    
    class Node* temp = toNode(this->root);
    queue<Node*> s;
    s.push(temp);

    int start_id = 0;

    while(!s.empty()) {
        temp = s.front();
        s.pop();
        temp->id = start_id++;

        if (temp->is_DataNode())
            toDataNode(temp)->printDataNode();
        else
            toIndexNode(temp)->printIndexNode();
            
        if (!temp->is_Leaf()) {
            // Go through all its child
            // Push all its child into queue
            for(int i = 0; i < toIndexNode(temp)->get_child_size(); i++) {
                s.push(toNode(toIndexNode(temp)->get_child_at_index(i)));
            }
        }
    }
}

void IndexNode::adjustID() {
    for (int i = 0; i < this->child.size(); i++) {
        // Assign parent 
    }
}

// By default it will do pro_active_splitting
bool BPlusTree::insert_element(int key, bool pro_active_split) {
    if (this->root == NULL) {
        this->root = new DataNode(this->numDataPointers, this->numIndexPointers);
    }

    if (pro_active_split) {
        
        // Search till the leaf and split everything comes in between that is full
        // Special case : Splitting root, if it is full
        if (toNode(this->root)->is_Leaf()) {
            if (toDataNode(this->root)->is_full()) {
                toDataNode(this->root)->insert_element(key);
                DataNode* temp = toDataNode(this->root)->splitNode(2);
                this->root = toDataNode(this->root)->get_parent();
            }
            else {
                toNode(this->root)->insert_element(key);
            }
            return NO_ERROR;
        }
        
        if (toNode(this->root)->is_full()) {

            toIndexNode(this->root)->splitNode();
            this->root = toNode(this->root)->get_parent();
        }

        class Node* temp = toNode(this->root);
        // Search till the leaf and split everything comes in between that is full
        while (!temp->is_Leaf()) {
        
            if (temp->is_full()) {
                toIndexNode(temp)->splitNode();
                temp = temp->get_parent();
            }

            // Search for the appropriate child
            int position = toIndexNode(temp)->lower_bound_on_child(key);

            // Get next node to search on
            temp = toNode(toIndexNode(temp)->get_child_at_index(position));
        }

        // We got the leaf
        if (temp->is_Leaf()) {

            // If leaf is full
            if (temp->is_full()) {
                temp->insert_element(key);
                DataNode* temp = toDataNode(temp)->splitNode(2);
            }
            else {
                temp->insert_element(key);
            }
        }
    }
    else {
        // Find the leaf
        Node *temp = toNode(this->root);
        while (!temp->is_Leaf()) {
            temp = toNode(toIndexNode(temp)->get_child_at_index(toIndexNode(temp)->lower_bound_on_child(key)));
        }
        // We got the leaf -> "temp"
        if (temp->is_full()) {
            temp->insert_element(key);
            
            // Node that needed to be split again
            DataNode *pointed_node;

            // Iteratively go to parent if parent also overflows and split the parent
            while (temp->is_overflow()) {
                if (temp->is_DataNode())
                    pointed_node = toDataNode(temp)->splitNode(3);
                else
                    toIndexNode(temp)->splitNode();
                if (temp == toNode(this->root))     this->root = temp->get_parent();
                if (temp->is_IndexNode()) {
                    toIndexNode(temp)->adjustParentPointer();
                }
                temp = temp->get_parent();
                if (temp->is_IndexNode()) {
                    toIndexNode(temp)->adjustParentPointer();
                }
            }

            temp = pointed_node;
            // Split the pointed node again
            do {
                if (temp->is_DataNode())
                    pointed_node = toDataNode(temp)->splitNode(2);
                else
                    toIndexNode(temp)->splitNode();
                if (temp == toNode(this->root))     this->root = temp->get_parent();
                if (temp->is_IndexNode()) {
                    toIndexNode(temp)->adjustParentPointer();
                }
                temp = temp->get_parent();
                if (temp->is_IndexNode()) {
                    toIndexNode(temp)->adjustParentPointer();
                }
            } while (temp->is_overflow());
        }
        else
            temp->insert_element(key);
    }
    return NO_ERROR;
}

// Search element at the leaf of BPlusTree
// Returns DataNode if found, else returns NULL
Node* BPlusTree::search_element(int key) {
    if (this->root == NULL)     return NULL;
    Node *temp = toNode(this->root);
    while(!temp->is_Leaf()) {
        // Search element in current node
        if(temp->search_element(key))
            return temp;
        temp = toNode(toIndexNode(temp)->get_child_at_index(toIndexNode(temp)->lower_bound_on_child(key)));
    }
    if (toDataNode(temp)->search_element(key))
        return temp;
    else
        return NULL;
}

/* *******************************************************************************
 * Member declarations of a Node
 * *******************************************************************************/

int Node::get_size() {
    return this->key.size();
}

int Node::get_depth() {
    return this->depth;
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

bool Node::is_DataNode() {
    return this->is_Leaf();
}

bool Node::is_IndexNode() {
    return !this->is_Leaf();
}

void Node::increase_depth() {
    this->depth++;
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

int Node::get_numDataPointers() {
    return this->numDataPointers;
}

int Node::get_numIndexPointers() {
    return this->numIndexPointers;
}

void IndexNode::adjustParentPointer() {
    for(int i = 0; i < this->child.size(); i++) {
        Node* child = toNode(this->child[i]);
        // Set child's parent = this
        child->parent = this;
    }
}

/* *******************************************************************************
 * Member declarations of a DataNode
 * *******************************************************************************/
DataNode::DataNode(int numDataPointers, int numIndexPointers, int depth, int id) {
    this->numDataPointers = numDataPointers;
    this->numIndexPointers = numIndexPointers;
    this->max_size = numDataPointers;
    this->min_size = (numDataPointers + 1)/2;
    this->depth = depth;
    this->parent = NULL;
    this->id = id;
}

class DataNode* DataNode::get_new_DataNode(int numDataPointers, int numIndexPointers) {
    return new DataNode(numDataPointers, numIndexPointers);
}

int DataNode::get_separator(int a, int b) {
    return a+1;
}

// Split the current node and send the middle element to the parent node
DataNode* DataNode::splitNode(int factor) {
    // Get the parent node
    class Node* parent = this->parent;

    /*********** Creating a right node **************/

    // Create a node for right node
    class DataNode *right_node = this->get_new_DataNode(this->get_numDataPointers(), this->get_numIndexPointers());
    if (factor == 3) {
        // Get keys for the right node
        right_node->key.insert(right_node->key.end(), this->key.begin()+2*(this->key.size()+2)/3, this->key.end());
    }
    else {
        // Get keys for the right node
        right_node->key.insert(right_node->key.end(), this->key.begin()+ceil(this->key.size()/2.0), this->key.end());
    }
    // Set parent of the node
    right_node->parent = parent;
    // Set depth of the new node
    right_node->depth = this->depth;
    // Set ID of node
    right_node->id = this->id + 1;
    
    /*********** Creating a left node **************/
    int key;
    if (factor == 3) {
        // key needed to send to parent
        key = this->get_separator(this->key[2*(this->key.size()+2)/3 - 1], this->key[2*(this->key.size()+2)/3]);

        // Keep this node as a left node, so delete right_keys
        this->key.erase(this->key.begin()+2*(this->key.size()+2)/3, this->key.end());
    }
    else {
        // key needed to send to parent
        key = this->get_separator(this->key[ceil(this->key.size()/2.0) - 1], this->key[ceil(this->key.size()/2.0)]);

        // Keep this node as a left node, so delete right_keys
        this->key.erase(this->key.begin()+ceil(this->key.size()/2.0), this->key.end());
    }

    /*********** Send the middle key to the parent **************/

    // If parent does not exists (Splitting the root case)
    if (parent == NULL) {

        // Create a new node for root
        parent = this->get_new_IndexNode(this->get_numDataPointers(), this->get_numIndexPointers());
        // Set left child of parent[0] = this
        toIndexNode(parent)->set_child_at_index(this);
        // set id of parent
        parent->id = this->id + 2;
        parent->depth = this->depth + 1;
    }

    int position = parent->insert_element(key);

    // Correctly inserted
    if (position != -1) {
        // Left child is already set at its required child position
        // Insert right child at parent[position+1]
        toIndexNode(parent)->set_child_at_index(right_node, position + 1);
    }
    else {
        cout << "Error splitting node ...\n";
        exit(-1);
    }

    // Set the parent of both the child
    this->parent = parent;
    right_node->parent = parent;

    return this;
}

// Print Node ID, Node keys and ID of children
void DataNode::printDataNode() {
    cout << "DataNode : " << this->id;
    cout << " Parent : ";
    if (this->parent == NULL) {
        cout << "NULL\t";
    }
    else {
        cout << this->parent->id << "\t";
    }
    cout << "Contents : ";
    for(int i = 0; i < this->get_size(); i++) {
        cout << this->key[i] << " ";
    }
    cout << endl;
}

/* *******************************************************************************
 * Member declarations of a IndexNode
 * *******************************************************************************/
IndexNode::IndexNode(int numDataPointers, int numIndexPointers, int depth, int id) {
    this->numDataPointers = numDataPointers;
    this->numIndexPointers = numIndexPointers;
    this->max_size = numIndexPointers;
    this->min_size = (numIndexPointers + 1)/2;
    this->depth = depth;
    this->parent = NULL;
    this->id = id;
}

int IndexNode::get_child_size() {
    return this->child.size();
}

int IndexNode::lower_bound_on_child (int key) {
    vector<int>::iterator it = lower_bound(this->key.begin(), this->key.end(), key);
    return it - this->key.begin();
}

void* IndexNode::get_child_at_index(int index) {
    return this->child[index];
}

// Returns true if the child is DataNode
bool IndexNode::isChildData() {
    // Check the child[0]
    return toNode(this->child[0])->is_DataNode();
}

// Print Node ID, Node keys and ID of children
void IndexNode::printIndexNode() {
    cout << "IndexNode : " << this->id;
    cout << "\tParent : ";
    if (this->parent == NULL)
        cout << "NULL\t";
    else 
        cout << this->parent->id << "\t";

    cout << "Contents : ";
    for(int i = 0; i < this->get_size(); i++) {
        cout << this->key[i] << " ";
    }
    cout << endl;
}

class IndexNode* Node::get_new_IndexNode(int numDataPointers, int numIndexPointers) {
    return new IndexNode(numDataPointers, numIndexPointers);
}

// Split the current node and send the middle element to the parent node
void IndexNode::splitNode() {
    // Get the parent node
    class Node* parent = this->parent;

    /*********** Creating a right node **************/

    // Create a node for right node
    class IndexNode *new_node = this->get_new_IndexNode(this->get_numDataPointers(), this->get_numIndexPointers());
    // Get keys for the right node
    new_node->key.insert(new_node->key.end(), this->key.begin()+ceil(this->key.size()/2.0) + 1, this->key.end());
    // Get child for right node
    new_node->child.insert(new_node->child.end(), this->child.begin()+ceil(this->child.size()/2.0), this->child.end());
    // Set parent of the node
    new_node->parent = parent;
    // Set depth of the new node
    new_node->depth = this->depth;
    // Set ID of node
    new_node->id = this->id + 1;
    
    /*********** Creating a left node **************/

    // key needed to send to parent
    int key = this->key[ceil(this->key.size()/2.0)];

    // Keep this node as a left node, so delete right_keys and child from the current node
    this->key.erase(this->key.begin()+ceil(this->key.size()/2.0), this->key.end());
    this->child.erase(this->child.begin()+ceil(this->child.size()/2.0), this->child.end());

    /*********** Send the middle key to the parent **************/

    // If parent does not exists (Splitting the root case)
    if (parent == NULL) {

        // Create a new node for root
        parent = this->get_new_IndexNode(this->get_numDataPointers(), this->get_numIndexPointers());
        // Set left child of parent[0] = this
        toIndexNode(parent)->child.push_back(this);
        // set id of parent
        parent->id = this->id + 2;
        parent->depth = this->depth + 1;
    }
    
    int position = parent->insert_element(key);

    // Correctly inserted
    if (position != -1) {
        // Left child is already set at its required child position
        // Insert right child at parent[position+1]
        toIndexNode(parent)->child.insert(toIndexNode(parent)->child.begin() + position + 1, new_node);
    }
    else {
        cout << "Error splitting node ...\n";
        exit(-1);
    }

    this->adjustParentPointer();
    new_node->adjustParentPointer();

    // Set the parent of both the child
    this->parent = parent;
    new_node->parent = parent;
}

void IndexNode::set_child_at_index(void *child, int position) {
    if (position == -1) {
        // Insert at the end
        this->child.push_back(child);
    }
    else {
        this->child.insert(this->child.begin() + position, child);
    }
}

/* *******************************************************************************
 * Main Begins
 * *******************************************************************************/

int main() {
	BPlusTree* tree = NULL;
	int key,choice;
	int numIndexPointers, numDataPointers;
	//node* searchResult;
	int searchResultIndex;

	do {
		cout << "1. Create new tree" << endl;
		cout << "2. Add new key to the tree" << endl;
		cout << "3. Search a key in the tree" << endl;
		cout << "4. Print the tree" << endl;
		cout << "Any other choice to exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		cout << "You entered " << choice << endl;

		switch(choice) {

		case 1:
			if(tree != NULL) {
				delete tree;
			}
			cout << "Enter number of Index pointers: ";
			cin >> numIndexPointers;
			cout << "Enter number of Data pointers: ";
			cin >> numDataPointers;

			if (numIndexPointers % 2 == 0 || numDataPointers % 2) {
				cout << "Note:: Index pointers must be in odd number and Data pointers must be in even number" << endl;
				// exit(0);
			}
			tree = new BPlusTree(numDataPointers, numIndexPointers);
			break;

		case 2:
			if(tree == NULL) {
				cout << "Create a new tree first" << endl;
				break;
			}
			cout << "Enter key to add ";
			cin >> key;
			tree->insert_element(key);
			break;
            
		case 4:
			if(tree == NULL) {
				cout << "Create a new tree first" << endl;
				break;
			}
			tree->printBTree();
			break;

		default:
			return 0;
		}
	} while(true);
	return 0;
}