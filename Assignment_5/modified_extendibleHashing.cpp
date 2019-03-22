#include <iostream>
#include <algorithm>
#include "modified_extendibleHashing.h"
using namespace std;

directory::directory(int global_depth, int bucket_size) {
    this->global_depth = global_depth;
    this->bucket_size = bucket_size;
    for (int i = 0; i < (1<<this->global_depth); i++) {
        class Bucket *new_bucket = this->get_new_bucket();
        this->bucket_list.push_back(new_bucket);
    }
}

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
    if (nbits)
        return value & (1<<nbits) - 1;
    else {
        return value & (1<<this->global_depth) - 1;
    }
}

int directory::get_size() {
    return this->bucket_list.size();
}

class Bucket* directory::get_bucket(int value, int nbits) {
    int hash_value = this->hash(value, nbits);
    return this->bucket_list[hash_value];
}

void directory::add_bucket(class Bucket *new_bucket) {
    this->bucket_list.push_back(new_bucket);
}

class Bucket* directory::get_new_bucket() {
    class Bucket* new_bucket = new class Bucket(this->bucket_size);
    return new_bucket;
}

bool directory::is_other_overflow(class Bucket *cur_bucket) {
    for (int i = 0; i < (1<<this->global_depth); i++) {
        // Get the ith bucket
        class Bucket* ith_bucket = this->get_bucket(i);
        // Check the overflow of buckets other than current_bucket
        if (cur_bucket != ith_bucket) {
            if (!ith_bucket->isOverflow()) 
                return false;
        }
    }
    // Overflowed occurred in all the other buckets
    return true;
}

void directory::add_value(int value) {
    class Bucket* b1 = this->get_bucket(value);
    // If Bucket is not full
    if(!b1->isfull()) {
        b1->add_value(value);
    }
    else {
        // If Bucket is full
        // Check if every other bucket has overflow bucket or not except current bucket
        if (this->is_other_overflow(b1)) {
            cout << "To be added : " << value << endl;
            cout << "Hash value : " << this->hash(value) << endl;
            cout << "Global depth : " << this->global_depth << endl;
            // Add the value to the bucket and split the directory
            b1->add_value(value);
            // Split all the buckets and redistribute each bucket
            this->split_directory();
        }
        else {
            // Add value to existing bucket (add overflow if needed)
            b1->add_value(value);
        }
    }
}

// Redistribute the keys
void directory::redistribute_keys(vector<int> keys) {
    for (int i = 0; i < keys.size(); i++) {
        this->get_bucket(keys[i], this->global_depth + 1)->add_value(keys[i]);
    }
}

// Split all the buckets and redistribute each bucket
void directory::split_directory() {
    for (int i = 0; i < (1<<this->global_depth); i++) {
        this->add_bucket(this->get_new_bucket());

        // Get all the keys associated to ith_bucket
        vector<int> gather_keys = this->get_bucket(i)->get_values();

        // Empty the ith_bucket
        this->get_bucket(i)->empty_bucket();

        // Distribute the keys gather_keys
        this->redistribute_keys(gather_keys);
    }
    // increase global depth
    this->increase_global_depth();
    cout << "Now global depth : " << this->global_depth << endl;
}

void directory::print() {
    for (int i = 0; i < this->get_size(); i++) {
        class Bucket* b = this->get_bucket(i);
        cout << "Bucket number " << i+1 << ": ";
        b->print();
        cout << endl;
    }
}

bool directory::search(int key) {
    // Get the Bucket in which key may present
    class Bucket* b = this->get_bucket(key);
    // search through the Bucket
    return b->search(key);
}

// Delete an element from the hash table
bool directory::deleteElement(int key) {
    // If key is not found
    if(!search(key))
        return false;

    // else Key is found
    // get the bucket in which key is stored
    // and delete the element from that bucket
    if (this->get_bucket(key)->deleteElement(key) == key)
        // deleted successfully
        return true;
    else
        // Got some error while deleting element
        return false;
}
/******************************************************************************
 * Member functions declaration of Class Bucket
 * ****************************************************************************/


Bucket::Bucket(int max_bucket_size) {
    this->max_size = max_bucket_size;
    this->next = NULL;
}

bool Bucket::isfull() {
    return this->get_size() == get_max_size();
}

bool Bucket::isempty() {
    return this->get_size() == 0;
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


class Bucket* Bucket::get_new_bucket() {
    return new class Bucket(this->get_max_size());
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

bool Bucket::isOverflow() {
    return this->next != NULL;
}

// Return exact deleted key if deleted successfully
// else return a different key (= key - 1)
int Bucket::deleteElement(int key) {
    // Search the element in the bucket
    vector<int>::iterator it = find(this->value.begin(), this->value.end(), key);

    // If found in the bucket -> delete it
    if (it != this->value.end()) {
        this->value.erase(it);

        // If there is overflow bucket, get an element from there to accomodate it into current bucket
        if (this->isOverflow()) {
            int get_element = this->next->deleteElement(this->next->value[0]);
            this->add_value(get_element);

            // After deleting, delete the overflow bucket if found empty
            if (this->next->isempty()) {
                delete this->next;
                this->next = NULL;
            }
        }
    }
    // else check in the overflow bucket
    else {
        if (this->isOverflow())
            int deletedElement = this->next->deleteElement(key);
        else
        // there is no overflow bucket
            return key - 1;

        // If not deleted successfully from the overflow bucket
        return key - 1;

        // After deleting, delete the overflow bucket if found empty
        if (this->next->isempty()) {
            delete this->next;
            this->next = NULL;
        }
    }
    // Deleted successfully
    return key;
}