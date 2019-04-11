#include "heapFile.h"
#include <iostream>

HeapFile::HeapFile(int pageSize) {
    this->pageSize = pageSize;
}

HeapFile::HeapFile(int numPages, int pageSize) {
    this->pageSize = pageSize;
    for(size_t i = 0; i < numPages; i++) {
        Page newPage(pageSize);
        this->addPage(newPage);
    }
}

int HeapFile::getPageNum() {
    return this->heapFile.size();
}

void HeapFile::addPage(Page page) {
    this->heapFile.push_back(page);
}

void HeapFile::insertRecord(int Rid, int size) {
    bool inserted = false;
    
    // Search over list of pages and check if it can accomodate the record

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        if (it->canAccomodate(size)) {
            it->insertRecord(Rid, size);
            inserted = true;
            break;
        }
    }
    
    // If no page can accomodate the record, create a new page

    if (!inserted) {
        // Create new page
        Page newPage(this->pageSize);
        // Insert record into this page
        newPage.insertRecord(Rid, size);
        // Insert it into heapfile
        this->addPage(newPage);
    }
}

// Returns true if deleted successfully
bool HeapFile::deleteRecord(int Rid) {
    bool deleted = false;
    
    // Iterate over pages and find the record

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        if (it->searchRecord(Rid)) {
            it->deleteRecord(Rid);
            deleted = true;
            break;
        }
    }
    
    // Returns if not deleted successfully
    return deleted;
}

bool HeapFile::searchRecord(int Rid) {

    // Iterate over pages and find the record

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        if (it->searchRecord(Rid)) {
            return true;
        }
    }
    return false;
}

void HeapFile::print() {
    int pageNum = 1;

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        std::cout << "Page " << pageNum++ << " : ";
        it->print();
    }

    std::cout << std::endl;
}


Page::Page(int pageSize) {
    this->pageSize = pageSize;
    this->availableSize = pageSize;
}

bool Page::searchRecord(int Rid) {

    // Iterate over slots and check validity and RecordId

    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (it->isValid() && it->matchRecord(Rid)) {
            return true;
        }
    }
    
    return false;
}

bool Page::canAccomodate(int size) {

    // If available size >= size + new_slot_size
    if (availableSize >= size + this->getSlotSize())
        return true;
    
    // Check in the available slots

    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (!it->isValid() && it->size() >= size) {
            return true;
        }
    }
    
    return false;
}

void Page::deleteRecord(int Rid) {

    // Iterate over slots and check validity and RecordId

    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (it->isValid() && it->matchRecord(Rid)) {
            it->resetValidBit();
            return;
        }
    }
}

int Page::getSlotSize() {
    return SLOT_SIZE;
}

void Page::insertRecord(int Rid, int size) {
    int endPos = 0;

    // Check if can accomodate in existing slots
    
    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (!it->isValid() && it->size() >= size) {
            it->setRecordId(Rid);
            it->setOccupiedSize(size);
            it->setValidBit();
            return;
        }
        endPos = it->getEndPos();
    }

    // If cannot accomodate, create a new slot
    Slot newSlot(size);
    newSlot.setRecordId(Rid);
    newSlot.setStartPos(endPos);
    newSlot.setOccupiedSize(size);
    newSlot.setValidBit();

    this->directory.push_back(newSlot);

    // Update available size in page
    this->availableSize -= size + this->getSlotSize();
}

void Page::print() {
    std::cout << "Records : ";
    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        it->print();
    }
    std::cout << std::endl;
}

Slot::Slot(int size) {
    this->total_size = size;
}

int Slot::getEndPos() {
    return this->start_pos + this->total_size;
}

bool Slot::isValid() {
    return this->validBit;
}

bool Slot::matchRecord(int Rid) {
    return this->record_id == Rid;
}

void Slot::resetValidBit() {
    this->validBit = false;
}

void Slot::setValidBit() {
    this->validBit = true;
}

void Slot::setOccupiedSize(int size) {
    this->occupied_size = size;
}

void Slot::setRecordId(int Rid) {
    this->record_id = Rid;
}

void Slot::setStartPos(int start_pos) {
    this->start_pos = start_pos;
}

int Slot::size() {
    return this->total_size;
}

void Slot::print() {
    // Print format
    // < Rid, startPos, occupiedSize - Gap, valid >
    std::cout << "< " << this->record_id << ", " << this->start_pos << ", " << this->occupied_size << " - ";
    std::cout << this->total_size - this->occupied_size << ", ";
    if (this->isValid()) std::cout << "True >";
    else std::cout << "False >";
}