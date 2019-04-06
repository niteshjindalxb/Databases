#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <climits>
#include <algorithm>


using namespace std;

/*********************************************************************************/
// common.h
/*********************************************************************************/
#define ERROR(error_msg) { std::cout << error_msg << std::endl; exit(1); }

int PAGE_SIZE;
int NUM_FRAMES;
int NUM_PAGES = 10000;
int FRAME_SIZE;
int k_buffer;

/*********************************************************************************/
// record.h
/*********************************************************************************/
class record
{
private:
    int value;
public:
    int getval() {        return this->value;       }
    void setval(int value) {        this->value = value;       }
    int display() {     std::cout << value << " ";  }
};
/*********************************************************************************/
// page.h
/*********************************************************************************/
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
/*********************************************************************************/
// frame.h
/*********************************************************************************/
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
/*********************************************************************************/
// mainMemory.h
/*********************************************************************************/
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
    void setNumFrames(int x);
    record extractMin(int &framePos, bool &empty, bool &valid);
    std::vector<int> getEmptyFramePos();
};
/*********************************************************************************/
// mainMemory.cpp
/*********************************************************************************/
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
    this->emptyBit.push_back(false);
    // Add page to last unoccupied frame
    Frame newFrame;
    // std::cout << "Added new frame\n";
    newFrame.addPageToFrame(page);
    this->FrameList.push_back(newFrame);
}

bool MainMemory::isFull() {
    return this->FrameList.size() >= this->numFrames;
}

void MainMemory::free() {
    this->FrameList.clear();
    this->emptyBit.clear();
}

int MainMemory::getFrameSize() {
    return this->FrameSize;
}

int MainMemory::getNumFrames() {
    return this->numFrames;
}

void MainMemory::setNumFrames(int x) {
    this->numFrames = x;
}

std::vector<int> MainMemory::getEmptyFramePos() {
    std::vector<int> result;
    for(size_t i = 0; i < this->emptyBit.size(); i++) {
        if (this->emptyBit[i])
            result.push_back(i);
    }
    return result;
}

// Extract minimum record from the frame list in Main Memory
record MainMemory::extractMin(int &framePos, bool &empty, bool &valid) {
    framePos = 0;
    record min;
    min.setval(INT_MAX);
    for(size_t i = 0; i < this->size(); i++) {
        Frame *frame = &this->FrameList[i];
        if (frame->size() > 0) {
            record r = frame->getRecordAt(0);
            if (min.getval() > r.getval()) {
                min = r;
                framePos = i;
            }
            valid = true;
        }
    }
    if (this->FrameList[framePos].size() > 0) {
        this->FrameList[framePos].popFront();

        if (this->FrameList[framePos].size() == 0) {
            this->emptyBit[framePos] = true;
            empty = true;
        }
    }
    return min;
}

// Convert frame into pages
// Note: A frame can contains multiple pages
std::vector<Page> MainMemory::frameToPages(Frame frame) {
    std::vector<Page> allPages;
    // Get number of pages in given frame
    // int numPages = frame.size()/this->PageSize;

    // for(size_t i = 0; i < numPages; i++) {
        Page newPage;
        newPage.addRecordtoPage(frame.getRecordBetween(0, frame.size()));
        allPages.push_back(newPage);
    // }
    return allPages;
}

int MainMemory::size() {
    return this->FrameList.size();
}
/*********************************************************************************/
// disk.h
/*********************************************************************************/
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
    void readDiskFile(int numEntries);
};

// Map [pass] = <run, Disk>
map<int, pair<int, Disk>> diskMap;

/*********************************************************************************/
// disk.cpp
/*********************************************************************************/
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

//creates DiskFile by reading input file (as records) through cin
void Disk::readDiskFile(int numEntries){
    int count = 0;
    int input;
    std::vector<record> rVec;

    bool flag = false;

    for(size_t i = 0; i < numEntries; i++) {
        cin >> input;
        flag = false;
        record r;
        r.setval(input);
        rVec.push_back(r);
		count ++;

        if (count == PAGE_SIZE) {
            Page page;
            page.addRecordtoPage(rVec);
            count = 0;
            rVec.clear();
            this->addPage(page);
            flag = true;
        }
    }

    if (!flag) {
        Page page;
        page.addRecordtoPage(rVec);
        this->addPage(page);
    }
}
/*********************************************************************************/
// externalmergesort.h
/*********************************************************************************/
class ExternalMergeSort
{
public:
    void sort(MainMemory &mainMemory, Disk &disk);
};
/*********************************************************************************/
// externalmergesort.cpp
/*********************************************************************************/
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
    /*********************************************************************************/
    // Till now, each page is sorted at its own
    /*********************************************************************************/
    
    // Now each page is sorted
    // Merge pages to sort the disk completely
    int run = 1;
    int pass = 0;

    int initial_frames = mainMemory.getNumFrames();
    // cout << "run = " << run << endl;
    while (run < disk.size()) {
        // Assuming frame size = page size
        // Take { mainMemory.size()-1 } pages from disk and start sorting 

        if (run == 1) {
            mainMemory.setNumFrames(initial_frames+1);
        }
        else {
            mainMemory.setNumFrames(initial_frames/k_buffer);
        }

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
        run = min(run * (mainMemory.getNumFrames() - 1), disk.size());
        // cout << "run = " << run << endl;
        pass ++;
        pair<int, Disk> state = make_pair(run, disk);
        diskMap[pass] = state;
        // std::cout << "here increases run : " << run << "\n";
        // std::cout << "run : " << run << std::endl;
    }
}


std::vector<record> getRandomPage(int size) {
    vector<record> r(size);
    for(size_t i = 0; i < r.size(); i++) {
        r[i].setval(rand() % 100);
    }
    return r;
}

int main(int argc, char const *argv[])
{
    int page_size;
    int numFrames;
    int k;
    int numEntries;
    
	cin >> page_size;
	cin >> numFrames;
	cin >> k;
	cin >> numEntries;

    PAGE_SIZE = page_size;
    NUM_FRAMES = numFrames;
    FRAME_SIZE = PAGE_SIZE;
    k_buffer = k;

	//create main memory of x frames
	MainMemory mm;

	//create a file by taking input from cin
	Disk f;
	f.readDiskFile(numEntries);
	// f.display();

	ExternalMergeSort e;
	
	//call 2 way externalmerge sort
	e.sort(mm, f);

	//output file by using cout
	// f.display(); 
    
    int query_pass = 0;
    int query_page = 0;
    cin >> query_pass;
    while (query_pass != -1) {
        cin >> query_page;
        if (diskMap.count(query_pass) != 0) {
            if (query_page <= diskMap[query_pass].second.size()) {
                cout << "Pass " << query_pass << " : ";
                cout << "Page " << query_page << " : ";
                diskMap[query_pass].second.getPageAt(query_page-1).display();
                // cout << " Run " << diskMap[query_pass].first;
                cout << "\n";
            }
            else {
                cout << "Pass " << query_pass << " : ";
                cout << "Page " << query_page << " : Invalid Page number\n";
            }
        }
        else {
            cout << "Pass " << query_pass << " : ";
            cout << "Page " << query_page << " : ";
            cout << "Invalid pass number\n";
        }
        cin >> query_pass;
    }
    
    return 0;
}
