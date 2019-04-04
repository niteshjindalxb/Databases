#include <vector>
#include "mainMemory.h"

Frame MainMemory::getFrameAt(int index) {
    if (index > this->FrameList.size())
        ERROR("Required Frame not available");
    return FrameList[index];
}

void MainMemory::setFrameAt(int index, Frame frame) {
    if (index > this->FrameList.size())
        ERROR("Cannot set frame at required index. Index overflowed");
    this->FrameList[index] = frame;
}

void MainMemory::sortFrameAt(int index) {
    if (index > this->FrameList.size())
        ERROR("Frame index overflowed");
    this->FrameList[index].sort();
}

void MainMemory::display() {
    for (int i = 0; i < this->FrameList.size(); i++) {
        std::cout << "Frame " << i+1 << " : ";
        this->FrameList[i].display();
        std::cout << std::endl;
    }
}

void MainMemory::addPagetoMemory(Page page) {
    // If last occupied frame is not full, add this page to this frame
    if (this->FrameList.size() > 0) {
        Frame lastFrame = this->FrameList[this->FrameList.size() - 1];
        if (!lastFrame.isFull()) {
            lastFrame.addPageToFrame(page);
            return;
        }
        else {
            // Add page to last unoccupied frame
            Frame newFrame;
            // std::cout << "Added new frame\n";
            newFrame.addPageToFrame(page);
            this->FrameList.push_back(newFrame);
        }
    }
    else {
        // Add page to last unoccupied frame
        Frame newFrame;
        // std::cout << "Added new frame\n";
        newFrame.addPageToFrame(page);
        this->FrameList.push_back(newFrame);
    }
}

bool MainMemory::isFull() {
    return this->FrameList.size() >= this->numFrames;
}

void MainMemory::makeEmpty() {
    this->FrameList.erase(std::begin(this->FrameList), std::end(this->FrameList));
}

// Convert frame into pages
// Note: A frame can contains multiple pages
std::vector<Page> MainMemory::frameToPages(Frame frame) {
    std::vector<Page> allPages;
    // Get number of pages in given frame
    int numPages = frame.size()/this->PageSize;

    for(size_t i = 0; i < numPages; i++) {
        Page newPage;
        newPage.addRecordtoPage(frame.getRecordBetween(i*this->PageSize, (i+1)*this->PageSize));
        allPages.push_back(newPage);
    }
    return allPages;
}

int MainMemory::size() {
    return this->FrameList.size();
}