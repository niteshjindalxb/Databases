#include<vector>

class directory
{
private:
    int global_depth;
    std::vector<class bucket*> node;
public:
    directory(int global_depth = 1);
    ~directory();
    void increase_global_depth();
    void decrease_global_depth();
    int get_global_depth();
    class bucket* get_new_bucket(int local_depth = 1);
    void add_node(class bucket *);
    void set_node(int, class bucket *);
    class bucket* get_node(int);
    int hash(int, int);
    void add_value(int);
    void split_directory();
    int get_size();
};

directory::directory(int global_depth = 1) {
    this->global_depth = global_depth;
    class bucket *b1 = this->get_new_bucket();
    class bucket *b2 = this->get_new_bucket();
    this->node.push_back(b1);
    this->node.push_back(b2);
}

directory::~directory() {
    for (int i = 0; i < 1<<this->global_depth; i++) {
        delete this->node[i];
    }
}

class bucket
{
private:
    int local_depth;
    std::vector<int> value;
    int max_size;

public:
    bucket(int local_depth = 1);
    int get_local_depth();
    bool isfull();
    int get_max_size();
    void increase_local_depth();
    void decrease_local_depth();
    void add_value(int);
    std::vector<int> get_values();
    int get_size();
    void empty_bucket();
};

bucket::bucket(int local_depth) {
    this->local_depth = local_depth;
    this->max_size = 2;
}