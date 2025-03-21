#pragma once

#define SAMPLE(name, ...) int main() { name(__VA_ARGS__); }
#define P99_PROTECT(...) __VA_ARGS__ 

int min(int a, int b) {
    return a > b ? b : a;
}

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
