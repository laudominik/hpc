#include <mpi.h>
#include <stdio.h>

#define DEBUG
#include <mpi_samples.h>

static void comms(int _) {
    int rank, world;
    MPI_Comm local_comm, inter_comm, intra_comm;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    int color = rank % 2;
    //            (MPI_Comm comm, int color, int key, MPI_Comm * newcomm)
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &local_comm);

    int local_rank, local;
    MPI_Comm_rank(local_comm, &local_rank);
    MPI_Comm_size(local_comm, &local);

    LOG_WITH_RANK("local: %d \n", local_rank);


    if (!color) {
        MPI_Intercomm_create(local_comm, 0, MPI_COMM_WORLD, 1, 0, &inter_comm);
        int x = 3;
        MPI_Send(&x, 1, MPI_INT, local_rank, 0, inter_comm);
        LOG_WITH_RANK("local: %d sent message \n", local_rank);
    } else {
        MPI_Intercomm_create(local_comm, 0, MPI_COMM_WORLD, 0, 0, &inter_comm);
        int y = 0;
        LOG_WITH_RANK("local: %d will wait for message \n", local_rank);
        MPI_Recv(&y, 1, MPI_INT, local_rank, 0, inter_comm, MPI_STATUS_IGNORE);
        LOG_WITH_RANK("local: %d got message \n", local_rank);
    }

    MPI_Intercomm_merge(inter_comm, color, &intra_comm);

    int intra_rank, intra_size;
    MPI_Comm_rank(intra_comm, &intra_rank);
    MPI_Comm_size(intra_comm, &intra_size);

    LOG_WITH_RANK("local: %d intra: %d\n", local_rank, intra_rank);
    MPI_Comm_free(&local_comm);
    MPI_Comm_free(&inter_comm);
    MPI_Comm_free(&intra_comm);
    MPI_Finalize();
}
SAMPLE(comms, 0)
