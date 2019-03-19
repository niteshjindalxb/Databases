#include <iostream>
#include "extendibleHashing.h"
using namespace std;

void directory::increase_global_depth() {
    this->global_depth ++;
}

void directory::decrease_global_depth() {
    this->global_depth --;
}

int directory::get_global_depth() {
    return this->global_depth;
}

int directory::hash(int value, int nbits = 0) {
    if (!nbits)
        return value && (1<<nbits - 1);
    else
        return value && (1<<this->global_depth - 1);
}

int directory::get_size() {
    return this->node.size();
}

class bucket* directory::get_node(int value) {
    int hash_value = this->hash(value);
    return this->node[hash_value];
}

void directory::set_node(int node_ptr, class bucket *new_bucket) {
    delete this->node[node_ptr];
    this->node[node_ptr] = new_bucket;
}

class bucket* directory::get_new_bucket(int local_depth) {
    class bucket* new_bucket = new class bucket(local_depth);
    return new_bucket;
}

void directory::add_value(int value) {
    class bucket* b1 = this->get_node(value);
    // If bucket is not full
    if(!b1->isfull())
    {
        b1->add_value(value);
    }
    else
        // If bucket is full
    {
        // Check if its localdepth < global depth
        if(b1->get_local_depth() < this->global_depth)
        {
            // get the location of first node pointing to same bucket
            int first_node = hash(value, b1->get_local_depth()+1);
            // get new bucket with local depth = b1.local_depth + 1
            class bucket *new_bucket = this->get_new_bucket(b1->get_local_depth() + 1);

            // set first half node to the new bucket
            // and remaining node will remain set to previous bucket
            for (int i = first_node; i < 1<<(this->global_depth-1); i += 1<<b1->get_local_depth())
                this->set_node(i, new_bucket);
            
            // Redistribute the items of old bucket
            vector<int> values ( b1->get_values() );
            values.push_back(value);

            // empty the old bucket to store new values and increase the local depth of old bucket
            b1->empty_bucket();
            b1->increase_local_depth();

            // insert values back to the directory
            for (vector<int>::iterator it = values.begin(); it != values.end(); ++it) {
                this->add_value(*it);
            }
        }
        else
        // If local_depth = global depth then split
        {
            // split the directory
            this->split_directory();
            // add value to new directory
            this->add_value(value);
        }
    }
}

void directory::split_directory() {
    for (int i = 0; i < 1<<this->global_depth; i++) {
        this->add_node(this->node[i]);
    }
    // increase global depth
    this->increase_global_depth();
}

/* Member function declaration of class bucket */
int bucket::get_local_depth() {
    return this->local_depth;
}

bool bucket::isfull() {
    return this->get_size() == get_max_size();
}

int bucket::get_size() {
    return this->value.size();
}

int bucket::get_max_size() {
    return this->max_size;
}

void bucket::increase_local_depth() {
    this->local_depth ++;
}

void bucket::decrease_local_depth() {
    this->local_depth --;
}

void bucket::add_value(int value) {
    if (!this->isfull())
        this->value.push_back(value);
    else
        cerr << "Error! Bucket overflow!\n";
}

std::vector<int> bucket::get_values() {
    return this->value;
}

void bucket::empty_bucket() {
    this->value.erase(value.begin(), value.end());
}