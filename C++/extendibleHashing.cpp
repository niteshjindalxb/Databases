#include <iostream>
#include <algorithm>
#include "extendibleHashing.h"
using namespace std;

directory::directory(int global_depth) {
    this->global_depth = global_depth;
    for (int i = 0; i < (1<<this->global_depth); i++) {
        class Bucket *new_bucket = this->get_new_bucket();
        this->bucket_list.push_back(new_bucket);
    }
}

// directory::~directory() {
//     for (int i = 0; i < 1<<this->global_depth; i++) {
//         delete this->bucket[i];
//     }
// }

void directory::increase_global_depth() {
    this->global_depth ++;
}

void directory::decrease_global_depth() {
    this->global_depth --;
}

int directory::get_global_depth() {
    return this->global_depth;
}

int directory::hash(int value, int nbits) {
    // cout << value & ((1<<nbits)-1) << endl;
    if (nbits)
        return value & (1<<nbits) - 1;
    else
    {
        // cout << "global depth used for calculating hash value:\t" << this->global_depth << endl;
        return value & (1<<this->global_depth) - 1;
    }
}

int directory::get_size() {
    return this->bucket_list.size();
}

class Bucket* directory::get_bucket(int value) {
    int hash_value = this->hash(value);
    return this->bucket_list[hash_value];
}

void directory::set_bucket(int bucket_ptr, class Bucket *new_bucket) {
    // delete this->bucket[bucket_ptr];
    this->bucket_list[bucket_ptr] = new_bucket;
}

void directory::add_bucket(class Bucket *new_bucket) {
    this->bucket_list.push_back(new_bucket);
}

class Bucket* directory::get_new_bucket(int local_depth) {
    class Bucket* new_bucket = new class Bucket(local_depth);
    return new_bucket;
}

void directory::add_value(int value) {
    class Bucket* b1 = this->get_bucket(value);
    // cout << "value outside = " << value << " and hash = " << this->hash(value) << "\n";
    // If Bucket is not full
    if(!b1->isfull())
    {
        // cout << "value = " << value << " and hash = " << this->hash(value) << "\n";
        b1->add_value(value);
    }
    else
        // If Bucket is full
    {
        // cout << "Bucket is full\n";
        // Check if its localdepth < global depth
        if(b1->get_local_depth() < this->global_depth)
        {
            // cout << "local_depth < global depth \n";
            // get the location of first bucket pointing to same Bucket
            int first_bucket = hash(value, b1->get_local_depth()+1);
            // get new Bucket with local depth = b1.local_depth + 1
            class Bucket *new_bucket = this->get_new_bucket(b1->get_local_depth() + 1);

            // set first half bucket to the new Bucket
            // and remaining bucket will remain set to previous Bucket
            for (int i = first_bucket; i < 1<<(this->global_depth-1); i += 1<<b1->get_local_depth())
                this->set_bucket(i, new_bucket);
            
            // Redistribute the items of old Bucket
            vector<int> values ( b1->get_values() );
            values.push_back(value);

            // empty the old Bucket to store new values and increase the local depth of old Bucket
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
    // cout << "Size before splitting:\t" << this->get_size() << "\n";
    for (int i = 0; i < 1<<this->global_depth; i++) {
        this->add_bucket(this->bucket_list[i]);
    }
    // cout << "Size after splitting:\t" << this->get_size() << "\n";
    // increase global depth
    this->increase_global_depth();
}

void directory::print() {
    for (int i = 0; i < this->get_size(); i++) {
        class Bucket* b = this->get_bucket(i);
        b->print();
    }
}

bool directory::search(int key) {
    // Get the Bucket in which key may present
    class Bucket* b = this->get_bucket(key);
    // search through the Bucket
    return b->search(key);
}

/* Member function declaration of class Bucket */
Bucket::Bucket(int local_depth) {
    this->local_depth = local_depth;
    this->max_size = BUCKET_SIZE;
}

int Bucket::get_local_depth() {
    return this->local_depth;
}

bool Bucket::isfull() {
    return this->get_size() == get_max_size();
}

int Bucket::get_size() {
    return this->value.size();
}

int Bucket::get_max_size() {
    return this->max_size;
}

void Bucket::increase_local_depth() {
    this->local_depth ++;
}

void Bucket::decrease_local_depth() {
    this->local_depth --;
}

void Bucket::add_value(int value) {
    if (!this->isfull())
    {
        // cout << "added value = " << value << "\n";
        this->value.push_back(value);
    }
    else
        cerr << "Error! Bucket overflow!\n";
}

std::vector<int> Bucket::get_values() {
    return this->value;
}

void Bucket::empty_bucket() {
    this->value.erase(value.begin(), value.end());
}

void Bucket::print() {
    for(int i = 0; i < this->get_size(); i++) {
        cout << this->value[i] << " ";
    }
    cout << endl;
}

bool Bucket::search(int key) {
    // Find in the vector
    return find(this->value.begin(), this->value.end(), key) != value.end();
}