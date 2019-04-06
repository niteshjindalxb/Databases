#include "externalMergeSort.h"
#include "common.h"
#include <iostream>
#include <random>

using namespace std;

std::vector<record> getRandomPage(int size) {
    vector<record> r(size);
    for(size_t i = 0; i < r.size(); i++) {
        r[i].setval(rand() % 100);
    }
    return r;
}

int main(int argc, char const *argv[])
{
    // srand(0);
    // int PageSize = NUM_PAGES;  // 10 records
    // ExternalMergeSort ext;
    // MainMemory mm;
    // Disk d;
    // // Insert into disks
    // for (int j = 0; j < 10; j++) {
    //     Page page;
    //     page.addRecordtoPage(getRandomPage(PageSize));
    //     d.addPage(page);
    // }
    // // d.display();
    // ext.sort(mm, d);
    // d.display();

    int x;
	//reads size of main memory in terms of number of frames available
	cin >> x;

	//create main memory of x frames
	MainMemory mm(x);

	//create a file by taking input from cin
	Disk f;
	f.readDiskFile();
	f.display();

	ExternalMergeSort e;
	
	//call 2 way externalmerge sort
	e.sort(mm, f);

	//output file by using cout
	f.display(); 
    
    return 0;
}
