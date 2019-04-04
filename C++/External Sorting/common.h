#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include "record.h"

#define ERROR(error_msg) { std::cout << error_msg << std::endl; exit(1); }

#define FRAME_SIZE 10
#define PAGE_SIZE 10
#define NUM_FRAMES 3
#define NUM_PAGES 10

// bool compare_record(record r1, record r2) {
//     return r1.getval() < r2.getval();
// }

#endif