#include <vector>
#include "common.h"
#include "frame.h"
#include "page.h"

#ifndef MAIN_MEMORY_H
#define MAIN_MEMORY_H

class MainMemory
{
private:
    int numFrames;
    int FrameSize;
    int PageSize;
    std::vector<bool> emptyBit;
    std::vector<Frame> FrameList;
public:
    MainMemory(int numFrames = NUM_FRAMES, int pageSize = PAGE_SIZE, int frameSize = FRAME_SIZE) {
        this->numFrames = numFrames;
        this->PageSize = pageSize;
        this->FrameSize = frameSize;
    }

    Frame getFrameAt(int index);
    void setFrameAt(int index, Frame frame);
    void addPagetoMemory(Page page);
    bool isFull();
    int size();
    void sortFrameAt(int index);
    std::vector<Page> frameToPages(Frame frame);
    void display();
    void free();
    int getFrameSize();
    int getNumFrames();
    record extractMin(int &framePos, bool &empty, bool &valid);
    std::vector<int> getEmptyFramePos();
};

#endif