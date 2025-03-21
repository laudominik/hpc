#include <mpi.h>
#include <stdio.h>

#include <mpi_samples.h>

static void quicksort(int arr[]){
    int rank, size;
    
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // TODO:
    MPI_Finalize();
}

static int arr[] = {3, 4, 1, 5};
SAMPLE(quicksort, arr);
