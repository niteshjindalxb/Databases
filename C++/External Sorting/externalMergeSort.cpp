#include <vector>
#include "externalMergeSort.h"

void ExternalMergeSort::sort(MainMemory &mainMemory, Disk &disk) {
    // Using mergeSort to sort Pages in disk using mainMemory

    int run = 0;
    // Sort each page individually

    // Load page from disk to mainMemory
    int page_num = 0;
    while (true) {
        int start_page = page_num;

        // std::cout << " diskSize : " << disk.size() << "\n";
        for(; page_num < disk.size(); page_num++) {    
            if (mainMemory.isFull()) {
                // std::cout << " page_num : " << page_num;
                // std::cout << " main memory size : " << mainMemory.size();
                break;
            }
            // std::cout << " main memory size : " << mainMemory.size();
            mainMemory.addPagetoMemory(disk.getPageAt(page_num));
        }
        // start_page to end_page (excluding end_page) is in main memory
        int end_page = page_num;

        // Sort each frame in main Memory
        for(size_t i = 0; i < mainMemory.size(); i++) {
            mainMemory.sortFrameAt(i);
        }
        // mainMemory.display();
        // Write back all the frames to disk
        for(size_t i = 0; i < mainMemory.size(); i++) {
            // Get a frame
            Frame frame = mainMemory.getFrameAt(i);
            std::vector<Page> allPages = mainMemory.frameToPages(frame);
            
            // Write all pages, back to disk
            for(size_t j = 0; j < allPages.size(); j++) {
                disk.setPageAt(start_page++, allPages[j]);
            }
        }
        
        // Empty the mainMemory
        mainMemory.makeEmpty();
        // If end_page == disk.size() then first pass is over
        if (end_page == disk.size())
            break;
    }
    
}