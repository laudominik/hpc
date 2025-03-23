#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#include <mpi_samples.h>

typedef struct {
    int id;
    double value;
    char name[10];
} Data;

void using_mpi_pack(int rank);
void using_custom_datatype(int rank);

int packing(int _) {
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    using_mpi_pack(rank);
    MPI_Barrier(MPI_COMM_WORLD);
    using_custom_datatype(rank);

    MPI_Finalize();
    return 0;
}
SAMPLE(packing, 0);

void using_mpi_pack(int rank) {
    if (rank == 0) {
        int id = 42;
        double value = 3.14159;
        char name[10] = "MPI Pack";

        int buffer_size = sizeof(int) + sizeof(double) + sizeof(name);
        void *buffer = malloc(buffer_size);
        int position = 0;

        MPI_Pack(&id, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&value, 1, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&name, 10, MPI_CHAR, buffer, buffer_size, &position, MPI_COMM_WORLD);

        MPI_Send(buffer, position, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
        free(buffer);

        LOG_WITH_RANK("Sent packed data\n");
    } else if (rank == 1) {
        int buffer_size = sizeof(int) + sizeof(double) + sizeof(char) * 10;
        void *buffer = malloc(buffer_size);
        int position = 0;

        MPI_Recv(buffer, buffer_size, MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int id;
        double value;
        char name[10];

        MPI_Unpack(buffer, buffer_size, &position, &id, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, buffer_size, &position, &value, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(buffer, buffer_size, &position, &name, 10, MPI_CHAR, MPI_COMM_WORLD);

        LOG_WITH_RANK("Received Packed Data -> id: %d, value: %f, name: %s\n", id, value, name);
        free(buffer);
    }
}

void using_custom_datatype(int rank) {
    MPI_Datatype MPI_DATA_TYPE;
    int block_lengths[3] = {1, 1, 10};
    MPI_Aint displacements[3];
    MPI_Datatype types[3] = {MPI_INT, MPI_DOUBLE, MPI_CHAR};

    displacements[0] = offsetof(Data, id);
    displacements[1] = offsetof(Data, value);
    displacements[2] = offsetof(Data, name);

    MPI_Type_create_struct(3, block_lengths, displacements, types, &MPI_DATA_TYPE);
    MPI_Type_commit(&MPI_DATA_TYPE);

    if (rank == 0) {
        Data data = {99, 2.71828, "MPI Type"};
        MPI_Send(&data, 1, MPI_DATA_TYPE, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        Data received_data;
        MPI_Recv(&received_data, 1, MPI_DATA_TYPE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1: Received Struct -> id: %d, value: %f, name: %s\n",
               received_data.id, received_data.value, received_data.name);
    }

    MPI_Type_free(&MPI_DATA_TYPE);
}