#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "linearHashing.h"
using namespace std;

linearHashing::linearHashing()
{
    this->ptr = 0;
    this->level = log2(INITIAL_NUM_BUCKETS);    // start with INITIAL_NUM_BUCKETS buckets
    
    for (int i = 0; i < (1<<this->level); i++) {
        // Create a bucket
        class Bucket* newBucket = this->get_new_bucket();
        // Push it into list of bucket
        this->bucket_list.push_back (newBucket);
    }
}

int linearHashing::get_level() {
    return this->level;
}

int linearHashing::hash(int value, int nbits) {
    return value & (1<<nbits) - 1;
}

int linearHashing::get_size() {
    return this->bucket_list.size();
}

void linearHashing::increase_level() {
    this->level ++;
}

void linearHashing::decrease_level() {
    this->level --;
}

void linearHashing::increase_ptr() {
    this->ptr ++;
}

void linearHashing::decrease_ptr() {
    this->ptr --;
}

void linearHashing::reset_ptr() {
    this->ptr = 0;
}

class Bucket* linearHashing::get_bucket(int value) {
    int hash_value = this->hash(value, this->level);
    if (hash_value < this->ptr) {
        // Check again with second hash function with nbits+1
        hash_value = this->hash(value, this->level+1);
    }
    // return the bucket with bucket_num = hash_value
    return this->bucket_list[hash_value];
}

void linearHashing::set_bucket(int bucket_ptr, class Bucket *new_bucket) {
    // delete this->bucket[bucket_ptr];
    this->bucket_list[bucket_ptr] = new_bucket;
}

void linearHashing::add_bucket(class Bucket *new_bucket) {
    this->bucket_list.push_back(new_bucket);
}

class Bucket* linearHashing::get_new_bucket() {
    class Bucket* new_bucket = new class Bucket;
    return new_bucket;
}

void linearHashing::split_bucket() {
    // Add new bucket into bucket_list
    this->add_bucket(this->get_new_bucket());

    // Get the items of old Bucket pointed by ptr
    vector<int> key ( this->get_bucket(this->ptr)->get_values() );

    // Delete everything from the bucket pointed by ptr
    this->get_bucket(this->ptr)->empty_bucket();

    // Redistribute the keys among bucket
    for (int i = 0; i < key.size(); i++) {
        // Get the bucket in which key[i] should go
        class Bucket* bucket = this->get_bucket(key[i]);
        bucket->add_value(key[i]);
    }
}

void linearHashing::add_value(int value) {
    class Bucket* b1 = this->get_bucket(value);
    // If Bucket is not full
    if(!b1->isfull()) {
        b1->add_value(value);
    }
    // If Bucket is full
    else {
        
        // Add value to the bucket and change the ptr while splitting a single bucket
        b1->add_value(value);

        // Split the bucket
        this->split_bucket();

        // If ptr reached the level then 
        // reset it to 0
        // and increase the level
        if (this->ptr == (1<<this->level)-1) {
            this->reset_ptr();
            this->increase_level();
        }
        // else
        else {
            // Increment the ptr
            this->increase_ptr();
        }
    }
}

void linearHashing::print() {
    for (int i = 0; i < this->get_size(); i++) {
        class Bucket* b = this->get_bucket(i);
        cout << "Bucket number " << i+1 << ": ";
        b->print();
        cout << endl;
    }
}

bool linearHashing::search(int key) {
    // Get the Bucket in which key may be present
    class Bucket* b = this->get_bucket(key);
    // search through the Bucket
    return b->search(key);
}

Bucket::Bucket()
{
    this->next = NULL;
    this->max_size = BUCKET_SIZE;
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

void Bucket::add_value(int value) {
    // cout << "added value = " << value << "\n";
    if (!this->isfull()) {
        this->value.push_back(value);
    }
    else {
        if (this->next == NULL)
            this->next = this->get_new_bucket();
        this->next->add_value(value);
    }
}

class Bucket* Bucket::get_new_bucket() {
    return new class Bucket;
}

void Bucket::empty_bucket() {
    this->value.erase(value.begin(), value.end());
    if (this->next != NULL) {
        this->next->empty_bucket();
        this->next = NULL;
    }
}

void Bucket::print() {
    for(int i = 0; i < this->get_size(); i++) {
        cout << this->value[i] << " ";
    }
    if (this->next != NULL) {
        cout << " Outflow: ";
        this->next->print();
    }
}

bool Bucket::isOverflow() {
    return this->next != NULL;
}

bool Bucket::search(int key) {
    // Find in the vector
    if (find(this->value.begin(), this->value.end(), key) != value.end()) {
        // If found return True
        return true;
    }
    else if (this->isOverflow()) {
        // Otherwise check in the overflow list if the bucket is overflowed
        return this->next->search(key);
    }
    return false;
}

std::vector<int> Bucket::get_values() {
    // If overflow is not happened
    if (!this->isOverflow())
        return this->value;
    else {
        std::vector <int>gather_value;
        vector<int> overFlow_list = this->next->get_values();
        gather_value.reserve(overFlow_list.size() + this->value.size());
        gather_value.insert (gather_value.end(), this->value.begin(), this->value.end());
        gather_value.insert (gather_value.end(), overFlow_list.begin(), overFlow_list.end());
        return gather_value;
    }
}
