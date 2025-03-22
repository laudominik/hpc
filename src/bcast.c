#include <mpi.h>
#include <stdio.h>

#define DEBUG
#include <mpi_samples.h>

static void bcast(int _) {
    int rank, world;
    int el = 0;
    int els[] = {3,1,4,7,1,5,1,8};

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    
    if (rank == 0) {
        el = 8;
    }
        LOG_WITH_RANK("before: %d\n", el);
        MPI_Bcast(&el, 1, MPI_INT, 0, MPI_COMM_WORLD);
        LOG_WITH_RANK("after: %d\n", el);
    
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        printf("sync'd");
    }
        el = 0;
        LOG_WITH_RANK("before: %d\n", el);
        MPI_Scatter(els, 1, MPI_INT, &el, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        LOG_WITH_RANK("after: %d\n", el);
    MPI_Finalize();
}
SAMPLE(bcast, 0)
