#include <mpi.h>
#include <stdio.h>

#include <mpi_samples.h>

static void collective(int _) {
    int rank, world;
    int global = 0;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);


    /*
    send data to a single process
    */

    MPI_Reduce(&rank, &global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("[rank: %d] MPI_Reduce: %d \n", rank, global);
  
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce(&rank, &global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("[rank: %d] MPI_Allreduce %d \n", rank, global);

    MPI_Finalize();
}
SAMPLE(collective, 0)
