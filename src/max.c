#include <mpi.h>
#include <omp.h>
#include <stdio.h>

#include <omp_samples.h>

static void second_max(int arr[], int n) {
    int workers = 4;
    int max_v = 0;
    int max2_v = 0;

    omp_set_num_threads(workers);

    #pragma omp parallel for reduction(max: max_v)
    for(int i = 0; i < n; i++){
        int local_max = arr[i];
        max_v = max_v < local_max ? local_max : max_v;
    }

    #pragma omp parallel for reduction(max: max2_v)
    for(int i = 0; i < n; i++){
        int local_max = arr[i];
        max2_v = max2_v < local_max && (local_max != max_v) ? local_max : max2_v;
    }

    printf("max %d max2 %d\n", max_v, max2_v);
}


int a[] = {
    1,      2,      3,      4,      5,
    2,      3,      1,      5,      6,
    4,      4,      3,      1,      1,
    8,      5,      1,      1,      1,
    16,     6,      3,      1,      1,
    32,     7,      1,      1,      1,
    64,     8,      3,      1,      1,
    128,    9,      1,      1,      1,
    256,    10,     3,      1,      1,
    512,    11,     1,      1000,      1,
};
SAMPLE(second_max, a, 50)
