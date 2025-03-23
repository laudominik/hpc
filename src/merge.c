#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <mpi_samples.h>

int comp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

static void msort(int a[], int n) {
    // large buffers sort
    int rank, world;
    int start, end;
    int size;
    int* sorted;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    size = n / world;
    start = size * rank;
    printf("[rank: %d] %d %d\n", rank, start, size);
    
    // sort the buffers any way
    qsort(a + start, size, sizeof(int), comp);

    if (rank == 0) {
        sorted = malloc(sizeof(int) * n);
    }

    MPI_Gather(
        a + start, size, MPI_INT, 
        sorted, size, MPI_INT, 
        0, MPI_COMM_WORLD
    );

    if (rank == 0) {
        int iter = 0;
        int partition_iters[16] = {0};

        // merge buffers
        for (int iter = 0; iter < n; iter++){
            int min = INT32_MAX;
            int chosen_rank = 0;
            for(int r = 0; r < world; r++) {
                int ix = partition_iters[r];
                if(ix >= size) {
                    continue;
                }
                int num = sorted[ix + size * r];
                if (num < min) {
                    min = num;
                    chosen_rank = r;
                }
            }
            partition_iters[chosen_rank]++;
            a[iter] = min;
        }

        for(int i = 0; i < n; i++){
            printf("%d ", a[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}


int a[] = {23, 12, 45, 89, 34, 67, 11, 78, 56, 90, 43, 21, 65, 32, 98, 87};
int n = 16;

SAMPLE(msort, a, n)
