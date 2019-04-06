#include <vector>
#include "mainMemory.h"
#include "disk.h"

#ifndef EXTERNAL_MERGESORT_H
#define EXTERNAL_MERGESORT_H

class ExternalMergeSort
{
public:
    void sort(MainMemory &mainMemory, Disk &disk);
};

#endif