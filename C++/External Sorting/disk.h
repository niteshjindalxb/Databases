#include <vector>
#include "common.h"
#include "page.h"

#ifndef DISK_H
#define DISK_H

class Disk
{
private:
    int NumPages;
    std::vector<Page> pageList;
    std::vector<bool> processedPage;

public:
    Disk(int numPages = NUM_PAGES) {
        this->NumPages = numPages;
    }

    Page getPageAt(int index);
    void setPageAt(int index, Page page);
    void addPage(Page page);
    int size();
    bool isFull();
    void display();
    void setMarkerAt(int index, bool value);
    bool isMarked(int index);
    void resetMarker();
};

#endif