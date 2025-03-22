#include <mpi.h>
#include <omp.h>
#include <stdio.h>

#include <omp_samples.h>

static void sum(int arr[], int n, int m) {
    // columnwise sum
    omp_set_num_threads(4);
    int* b = malloc(sizeof(int) * n);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        # pragma omp for schedule(dynamic)
        for (int i = 0; i < n; i++){
            int sum = 0;
            LOG_WITH_THREADID("col: %d", i);

            for(int j = 0; j < m; j++){
                sum += arr[i + j * n];
            }
            b[i] = sum;
        }

        #pragma omp barrier
        #pragma omp master
        {
            int sum = 0;
            for (int i = 0; i < n; i++){
                sum += b[i];
            }
            printf("%d \n", sum);
        }
    }

    // compare to normal sum
    int sum = 0;
    for(int i = 0; i < n * m; i++){
        sum += arr[i];
    }
    printf("%d\n", sum);
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
    512,    11,     1,      1,      1,
};
SAMPLE(sum, a, 5, 10)
