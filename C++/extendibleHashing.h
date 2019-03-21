#include<vector>

#ifndef EXTENDIBLE_HASHING_H
#define EXTENDIBLE_HASHING_H

#define BUCKET_SIZE 3
/* 
 * INITIAL_GLOBAL_DEPTH and INITIALP_LOCAL_DEPTH must be same
 * */
#define INITIAL_GLOBAL_DEPTH 2
#define INITIAL_LOCAL_DEPTH 2

class directory
{
private:
    int global_depth;
    std::vector<class bucket*> node;
public:
    directory(int global_depth = INITIAL_GLOBAL_DEPTH);
    // ~directory();
    void increase_global_depth();
    void decrease_global_depth();
    int get_global_depth();
    class bucket* get_new_bucket(int local_depth = INITIAL_LOCAL_DEPTH);
    void add_node(class bucket *);
    void set_node(int, class bucket *);
    class bucket* get_node(int);
    int hash(int, int nbits = 0);
    void add_value(int);
    void split_directory();
    int get_size();
    void print();
    bool search(int);
};


class bucket
{
private:
    int local_depth;
    std::vector<int> value;
    int max_size;

public:
    bucket(int local_depth = INITIAL_LOCAL_DEPTH);
    int get_local_depth();
    bool isfull();
    int get_max_size();
    void increase_local_depth();
    void decrease_local_depth();
    void add_value(int);
    std::vector<int> get_values();
    int get_size();
    void empty_bucket();
    void print();
    bool search(int);
};

#endif // EXTENDIBLE_HASHING_H