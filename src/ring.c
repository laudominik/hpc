#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpi_samples.h>

static void ring(int _) {
    int rank, world;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);


    // MPI_Send();

    // MPI_Recv()



    MPI_Finalize();
}
SAMPLE(ring, 0)
