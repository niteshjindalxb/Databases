#include <vector>

#ifndef BTREE_H
#define BTREE_H

#define DEFAULT_NUM_DATA 3

#define ERROR false
#define NO_ERROR true

class BTree
{
private:
    int numDataPointers;
    int depth;
    class Node* root;
    
public:
    BTree(int numDataPointers = DEFAULT_NUM_DATA);
    // ~BTree();
    bool insert_element(int key, bool pro_active_split = true);
    void printBTree();
    Node* search_element(int);
};

class Node
{
private:
    std::vector<int> key;
    std::vector<class Node*> child;
    class Node* parent;

    int id;
    int max_size;
    int min_size;
    int depth;

public:
    Node(int numDataPointers = DEFAULT_NUM_DATA, int depth = 0, int id = 0);
    // ~Node();
    int get_size();
    int get_child_size();
    int get_numDataPointers();
    int get_depth();
    bool is_full();
    bool is_overflow();
    bool is_Leaf();
    void increase_depth();
    class Node* get_parent();
    int get_key_at_index(int);
    int lower_bound_on_child (int);
    class Node* get_child_at_index(int);
    class Node* get_new_node(int numDataPointers = DEFAULT_NUM_DATA);
    int insert_element(int);
    bool search_element(int);
    void splitNode();
    void printNode();
};


#endif // end of BTREE_H