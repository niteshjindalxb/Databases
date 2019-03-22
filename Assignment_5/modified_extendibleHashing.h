#include<vector>

#ifndef MODIFIED_EXTENDIBLE_HASHING_H
#define MODIFIED_EXTENDIBLE_HASHING_H

#define BUCKET_SIZE 3
/* 
 * INITIAL_GLOBAL_DEPTH and INITIALP_LOCAL_DEPTH must be same
 * */
#define INITIAL_GLOBAL_DEPTH 2

class directory
{
private:
    int global_depth;
    int bucket_size;
    std::vector<class Bucket*> bucket_list;
public:
    directory(int global_depth = INITIAL_GLOBAL_DEPTH, int bucket_size = BUCKET_SIZE);
    // ~directory();
    void increase_global_depth();
    void decrease_global_depth();
    int get_global_depth();
    class Bucket* get_new_bucket();
    void add_bucket(class Bucket *);
    void set_bucket(int, class Bucket *);
    class Bucket* get_bucket(int, int nbits = 0);
    int hash(int, int nbits = 0);
    bool is_other_overflow(class Bucket *);
    void redistribute_keys(std::vector<int>);
    void split_directory();
    int get_size();
    void add_value(int);
    bool search(int);
    bool deleteElement(int);
    void print();
};


class Bucket
{
private:
    std::vector<int> value;
    int max_size;
    class Bucket* next;

public:
    Bucket(int max_bucket_size = BUCKET_SIZE);
    bool isfull();
    bool isempty();
    int get_max_size();
    void add_value(int);
    std::vector<int> get_values();
    class Bucket* get_new_bucket();
    int get_size();
    void empty_bucket();
    int deleteElement(int);
    void print();
    bool search(int);
    bool isOverflow();
};

#endif // MODIFIED_EXTENDIBLE_HASHING_H