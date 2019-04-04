#include "externalMergeSort.h"
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
    srand(0);
    int PageSize = 10;  // 10 records
    ExternalMergeSort ext;
    MainMemory mm;
    Disk d;
    // Insert into disks
    for (int j = 0; j < 10; j++) {
        Page page;
        page.addRecordtoPage(getRandomPage(PageSize));
        d.addPage(page);
    }
    ext.sort(mm, d);
    d.display();
    return 0;
}
