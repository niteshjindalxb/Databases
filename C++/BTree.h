#include <vector>

#ifndef BTREE_H
#define BTREE_H

#define DEFAULT_T 2

#define ERROR false
#define NO_ERROR true

class BTree
{
private:
    int t;
    int depth;
    class Node* root;
    
public:
    BTree(int t = DEFAULT_T);
    ~BTree();
    class BTree* createBTree(int t = DEFAULT_T);
    bool insert_element(int key, bool pro_active_split = true);
};

class Node
{
private:
    std::vector<int> key;
    std::vector<class Node*> child;
    class Node* parent;

    int max_size;
    int min_size;
    int depth;

public:
    Node(int t = DEFAULT_T, int depth = 0);
    ~Node();
    int get_size();
    int get_t();
    int get_depth();
    bool is_full();
    bool is_Leaf();
    void increase_depth();
    class Node* get_parent();
    int get_key_at_index(int);
    int lower_bound_on_child (int);
    class Node* get_child_at_index(int);
    class Node* get_new_node(int t = DEFAULT_T);
    int insert_element(int);
    bool search_element(int);
    void splitNode();
};


#endif // end of BTREE_H