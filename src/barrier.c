#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <mpi_samples.h>

#define DEBUG
#undef DEBUG

void barr(int rank, int world) {
    // master-slave
    if (rank == 0) {
        for (int i = 1; i < world; i++){
            MPI_Recv(0, 0, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int i = 1; i < world; i++){
            MPI_Send(0, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
        }   
    } else {
        MPI_Send(0, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(0, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void barr2(int rank, int world) {
    int node = rank + 1;
    int left = 2*node;
    int right = 2*node+1;
    int parent = node / 2;

    // 1. parents wait for their children
    if (left <= world) {
        #ifdef DEBUG
        LOG_WITH_RANK("Waiting for left child\n");
        #endif
        MPI_Recv(0, 0, MPI_INT, left-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        #ifdef DEBUG
        LOG_WITH_RANK("Received left child\n");
        #endif
    } 
    #ifdef DEBUG
    else {
        LOG_WITH_RANK("Leaf\n");
    }
    #endif

    if (right <= world) {
        #ifdef DEBUG
        LOG_WITH_RANK("Waiting for right child\n");
        #endif
        MPI_Recv(0, 0, MPI_INT, right-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        #ifdef DEBUG
        LOG_WITH_RANK("Received right child\n");
        #endif
    } 
    #ifdef DEBUG
    else {
        LOG_WITH_RANK("Leaf\n");
    }
    #endif

    // 2. children notify the parent
    if (rank != 0) {
        MPI_Send(0, 0, MPI_INT, parent-1, 0, MPI_COMM_WORLD);
    }

    // 3. children wait for the parent
    if (rank != 0) {
        #ifdef DEBUG
        LOG_WITH_RANK("Waiting for parent\n");
        #endif
        MPI_Recv(0, 0, MPI_INT, parent-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        #ifdef DEBUG
        LOG_WITH_RANK("Received parent\n");
        #endif
    }

    // 4. parent notifies the children
    if (left <= world) {
        MPI_Send(0, 0, MPI_INT, left-1, 0, MPI_COMM_WORLD);
    }
    if (right <= world) {
        MPI_Send(0, 0, MPI_INT, right-1, 0, MPI_COMM_WORLD);
    }
}

static void barrier(int _) {
    int rank, world;

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    LOG_WITH_RANK("before %f \n", MPI_Wtime());
    sleep(2*rank);
    barr2(rank, world);
    LOG_WITH_RANK("after %f \n", MPI_Wtime());

    MPI_Finalize();
}
SAMPLE(barrier, 0)
