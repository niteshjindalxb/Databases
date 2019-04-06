#include <vector>
#include <algorithm>
#include "record.h"
#include "common.h"
#include "page.h"

#ifndef FRAME_H
#define FRAME_H

class Frame
{
private:
    int FrameSize;
    std::vector<record> recordList;
public:
    Frame(int FrameSize = FRAME_SIZE) {
        this->FrameSize = FrameSize;
    }
    bool isFull() {
        return this->recordList.size() >= this->FrameSize;
    }
    void addPageToFrame(Page page) {
        std::vector<record> recordVec = page.getRecord();
        this->recordList.insert(std::end(this->recordList), std::begin(recordVec), std::end(recordVec));
    }
    void sort() {
        std::sort(this->recordList.begin(), this->recordList.end(), [](record r1, record r2) -> bool
            { 
                return r1.getval() < r2.getval(); 
            }
        );
    }
    int size() {
        return this->recordList.size();
    }
    std::vector<record> getRecordBetween(int start, int end) {
        std::vector<record> subVector(this->recordList.begin()+start, this->recordList.begin()+end);
        return subVector;
    }
    record getRecordAt(int index) {
        return this->recordList[index];
    }
    void display() {
        for (int i = 0; i < this->recordList.size(); i++) recordList[i].display();
    }
    void popFront() {
        if (!this->recordList.empty())
            this->recordList.erase(this->recordList.begin());
    }
};

#endif