#pragma once

#include <samples.h>

#ifdef DEBUG
#define LOG_WITH_RANK(text, ...) printf("[rank %d]:" text, rank, ##__VA_ARGS__)
#else
#define LOG_WITH_RANK(text, ...)
#endif


#define IX_TO_NODE(ix) ix + 1
#define NODE_TO_IX(node) node - 1

int min(int a, int b) {
    return a > b ? b : a;
}

// [start, end]
void get_start_end(int rank, int workers, int size, int* start, int* end) {  
    int remainder = size % workers;
    int count = size / workers;

    if (rank < remainder) {
        *start = rank * (count + 1);
        *end = *start + count;
    } else {
        *start = rank * count + remainder;
        *end = *start + (count - 1);
    }
}
