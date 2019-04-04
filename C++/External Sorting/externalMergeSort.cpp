#include <vector>
#include <algorithm>
#include "externalMergeSort.h"

// Using mergeSort to sort Pages in disk using mainMemory
void ExternalMergeSort::sort(MainMemory &mainMemory, Disk &disk) {

    // Sort each page individually

    // Load page from disk to mainMemory
    int page_it = 0;
    while (true) {
        int start_page = page_it;

        // std::cout << " diskSize : " << disk.size() << "\n";
        for(; page_it < disk.size(); page_it++) {    
            if (mainMemory.isFull()) {
                break;
            }
            // std::cout << " main memory size : " << mainMemory.size();
            mainMemory.addPagetoMemory(disk.getPageAt(page_it));
        }
        // start_page to end_page (excluding end_page) is in main memory
        int end_page = page_it;

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
        
        // Free the mainMemory
        mainMemory.free();
        // If end_page == disk.size() then first pass is over
        if (end_page == disk.size())
            break;
    }

    disk.display();
    std::cout << std::endl;
    
    // Now each page is sorted
    // Merge pages to sort the disk completely
    int run = 1;

    while (run < disk.size()) {
        // Assuming frame size = page size
        // Take { mainMemory.size()-1 } pages from disk and start sorting 

        // Clear the main memory
        mainMemory.free();
            
        int base = 0;

        while (base < disk.size()) {
            // std::cout << "base : " << base << "\n";
            // Clear the main memory
            mainMemory.free();
            
            // vector to keep track of pages of disk currently processing in main memory
            std::vector<int> pageInMemory;
            
            // Get k-1 pages from disk to mainMemory
            for(size_t i = 0; i < mainMemory.getNumFrames() - 1; i++) {
                if (base + i*run >= disk.size())
                    break;
                mainMemory.addPagetoMemory(disk.getPageAt(base + i*run));
                disk.setMarkerAt(base + i*run, true);
                pageInMemory.push_back(base + i*run);
            }

            Disk tempDisk;

            // Iterate over disk from base to min(base + run*(mainMemory.getNumFrames()-1), disk.size())
            for(size_t _ = base; _ < std::min(base + run*(mainMemory.getNumFrames()-1), disk.size()); _++) {
            
                // Start comparing each frame in mainMemory
                Page lastPage;

                // Build a page
                for(size_t i = 0; i < mainMemory.getFrameSize(); i++) {
                    int framePos;
                    bool empty = false;
                    bool valid = false;
                    // valid = true if minimum element is extracted successfully
                    record r = mainMemory.extractMin(framePos, empty, valid);
                    // mainMemory.display();
                    if (valid) {
                        std::vector<record> rVec(1, r);
                        lastPage.addRecordtoPage(rVec);

                        // Try to get a correspoding page from disk if frame[framePos] is empty
                        if (empty) {
                                // std::cout << "pageInMemory[" << framePos << "]+1 : " << pageInMemory[framePos]+1 << std::endl;
                                // std::cout << "run : " << run << std::endl;
                            if (pageInMemory[framePos] + 1 < base + (framePos+1)*run && pageInMemory[framePos]+1 < disk.size()) {
                                // More pages are there to be processed
                                Frame newFrame;
                                newFrame.addPageToFrame(disk.getPageAt(pageInMemory[framePos]+1));
                                mainMemory.setFrameAt(framePos, newFrame);
                                pageInMemory[framePos] ++;
                            }
                        }
                    }
                    // else
                    //     std::cout << "framePos : " << framePos << std::endl;
                }
                // Write the page into another temp tempDisk file
                // std::cout << base << std::endl;
                tempDisk.addPage(lastPage);
            }

            // tempDisk.display();
            // std::cout << std::endl;

            // Write tempDisk (0 to tempDisk.size()) back to disk ()
            int start_pos = base;
            for(size_t tempDisk_it = 0; tempDisk_it < tempDisk.size(); tempDisk_it++) {
                disk.setPageAt(start_pos, tempDisk.getPageAt(tempDisk_it));
                start_pos++;
            }
            base = std::min(base + run*(mainMemory.getNumFrames() - 1), disk.size());
        }
        run = run * (mainMemory.getNumFrames() - 1);
        // std::cout << "here increases run : " << run << "\n";
        // std::cout << "run : " << run << std::endl;
    }
}

/*
 *  To Do
 *      - Handle partial filled page
 * */