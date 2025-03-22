#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpi_samples.h>


static void unknown_size(int _) {
    int rank, world;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    if(rank == 0) {\
        int size = 3;
        void* buffer = malloc(size);
        MPI_Send (
            buffer, size, 
            MPI_CHAR, 
            1,
            0, MPI_COMM_WORLD
        );
        free(buffer);
    } else if (rank == 1) {
        int size;
        MPI_Status status;
        int buff[100] = {0};

        MPI_Recv(
            buff, 100,
            MPI_CHAR,
            0,
            0, MPI_COMM_WORLD,
            &status
        );

        MPI_Get_count(
            &status, MPI_CHAR, &size
        );

        printf("%d \n", size);
  
    }

    MPI_Finalize();
}
SAMPLE(unknown_size, 0)
