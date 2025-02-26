#include <mpi.h>
#include <stdio.h>

#include <mpi_samples.h>

static void hello(int _) {
    int rank, world;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    printf("Hello: rank %d, world: %d\n",rank, world);
    MPI_Finalize();
}
SAMPLE(hello, 0)
