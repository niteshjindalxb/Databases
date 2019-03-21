#include <vector>

#ifndef LINEAR_HASHING_H
#define LINEAR_HASHING_H

#define BUCKET_SIZE 2
#define INITIAL_NUM_BUCKETS 2

class linearHashing
{
private:
    std::vector<class Bucket*> bucket_list;
    int ptr;
    int level;  // "number of bits" required to denote the number of buckets after which ptr will set back to 0
public:
    linearHashing();
    int get_level();
    void increase_level();
    void decrease_level();
    void reset_ptr();
    void increase_ptr();
    void decrease_ptr();
    class Bucket* get_new_bucket();
    void add_bucket(class Bucket *);
    void set_bucket(int, class Bucket *);
    class Bucket* get_bucket(int);
    void split_bucket();
    int hash(int, int);
    void add_value(int);
    int get_size();
    void print();
    bool search(int);
};

class Bucket
{
private:
    std::vector <int> value;
    int max_size;
    class Bucket *next;

public:
    Bucket();
    bool isfull();
    int get_max_size();
    void add_value(int);
    std::vector<int> get_values();
    class Bucket* get_new_bucket();
    int get_size();
    void empty_bucket();
    void print();
    bool search(int);
    bool isOverflow();
};

#endif  // end of LINEAR_HASHING_H