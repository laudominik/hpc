#include <mpi.h>
#include <stdio.h>

#include <mpi_samples.h>

#define MIN_RANGE 0
#define MAX_RANGE 15
#define N MAX_RANGE - MIN_RANGE + 1

static void hist(const int a[], int n) {
    int rank, world;
    int hist[N] = { 0 };
    int workers;
    int start, end;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    workers = world - 1;

    if (rank == world - 1) {
        // master

        for (int i = 0; i < workers; i++) {
            int temp_hist[N] = {0};
            MPI_Recv(temp_hist, N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < N; j++) {
                hist[j] += temp_hist[j];
            }
        }

        for (int i = 0; i < N; i++){
            printf("%d -> %d \n", i, hist[i]);
        }

    } else {
        // slave
        get_start_end(rank, workers, n, &start, &end);

        printf("rank: %d n: %d workers: %d start: %d end:%d\n", rank, n, workers, start, end);

        for (int i = start; i <= end; i++){
            printf("ix %d \n", i+start);
            hist[a[i]]++;
        }

        MPI_Send(hist, N, MPI_INT, world - 1, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

int arg[] = {1, 2, 3, 1, 1, 1, 15, 1};
int n = 8;

SAMPLE(hist, arg, n);
