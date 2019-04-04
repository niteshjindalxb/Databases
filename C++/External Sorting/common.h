#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include "record.h"

#define ERROR(error_msg) { std::cout << error_msg << std::endl; exit(1); }

#define FRAME_SIZE PAGE_SIZE
#define PAGE_SIZE 10
#define NUM_FRAMES 5
#define NUM_PAGES 100

// bool compare_record(record r1, record r2) {
//     return r1.getval() < r2.getval();
// }

#endif