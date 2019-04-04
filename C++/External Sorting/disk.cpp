#include <vector>
#include "disk.h"

Page Disk::getPageAt(int index) {
    if (index > this->pageList.size())
        ERROR("Required Page not available");
    return pageList[index];
}

void Disk::setPageAt(int index, Page page) {
    if (index >= this->pageList.size())
        ERROR("Cannot set Page at required index. Index overflowed");
    this->pageList[index] = page;
}

bool Disk::isFull() {
    return this->pageList.size() == this->NumPages;
}

void Disk::addPage(Page page) {
    // std::cout << "Disk size : " << this->size() << " and maxSize : " << this->NumPages << std::endl;
    if (this->isFull())
        ERROR("Cannot add more pages. Disk is full");
    this->pageList.push_back(page);
    this->processedPage.push_back(false);
}

int Disk::size() {
    return this->pageList.size();
}

void Disk::display() {
    for(size_t i = 0; i < this->size(); i++) {
        // Display each pages
        std::cout << "Page " << i+1 << " : ";
        this->pageList[i].display();
        std::cout << "\n";
    }
}

void Disk::setMarkerAt(int index, bool value) {
    this->processedPage[index] = value;
}

bool Disk::isMarked(int index) {
    return this->processedPage[index];
}

void Disk::resetMarker() {
    for(size_t i = 0; i < this->processedPage.size(); i++) {
        this->setMarkerAt(i, false);
    }
}