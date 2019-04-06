#include <vector>
#include "record.h"
#include "common.h"

#ifndef PAGE_H
#define PAGE_H

class Page
{
private:
    int PageSize;
    std::vector<record> recordList;
public:
    Page(int PageSize = PAGE_SIZE) {        this->PageSize = PageSize;      }
    bool isFull() {        return this->recordList.size() == this->PageSize;    }
    std::vector<record> getRecord() {        return this->recordList;       }
    void addRecordtoPage(std::vector<record> r) {  this->recordList.insert(this->recordList.end(), r.begin(), r.end());    }
    void setRecordAt(int index, record r) {  this->recordList[index] = r;    }
    int size() {    return this->recordList.size();     }

    void display() {
        for(size_t i = 0; i < this->size(); i++) {
            this->recordList[i].display();
        }
    }
};

#endif