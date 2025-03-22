#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#undef DEBUG
#include <mpi_samples.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int comp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int partition(int arr[], int low, int high) {
    int p = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        while (arr[i] <= p && i <= high - 1) {
            i++;
        }
        while (arr[j] > p && j >= low + 1) {
            j--;
        }
        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[low], &arr[j]);
    return j;
}

static void quicksort(int arr[], int low, int high){
    int rank, world;
    int node, left, right, parent;
    int pi;
    int part[2] = {0};
    int left_temp_part[2] = {0};
    int right_temp_part[2] = {0};

    
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    node = rank + 1;
    left = 2 * node;
    right = 2 * node + 1;
    parent = node / 2;

    if (rank == 0) {
        part[0] = low;
        part[1] = high;
    } else if (node + 1 > world) {
        // doesn't have a sibling -> useless node
        LOG_WITH_RANK("Useless node");
        goto end;
    } 
    else {
        MPI_Recv(part, 2, MPI_INT, NODE_TO_IX(parent), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(arr + part[0], part[1] - part[0] + 1, MPI_INT, NODE_TO_IX(parent), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    LOG_WITH_RANK("part: %d %d\n", part[0], part[1]);

    if (left <= world && right <= world ) {
        LOG_WITH_RANK("splitting\n");
        pi = partition(arr, part[0], part[1]);
   
        left_temp_part[0] = part[0]; left_temp_part[1] = pi;
        MPI_Send(left_temp_part, 2, MPI_INT, NODE_TO_IX(left), 0, MPI_COMM_WORLD);
        MPI_Send(arr + left_temp_part[0], left_temp_part[1] - left_temp_part[0] + 1, MPI_INT, NODE_TO_IX(left), 0, MPI_COMM_WORLD);
    
        right_temp_part[0] = pi+1; right_temp_part[1] = part[1];
        MPI_Send(right_temp_part, 2, MPI_INT, NODE_TO_IX(right), 0, MPI_COMM_WORLD);
        MPI_Send(arr + right_temp_part[0], right_temp_part[1] - right_temp_part[0] + 1, MPI_INT, NODE_TO_IX(right), 0, MPI_COMM_WORLD);

        LOG_WITH_RANK("Waiting for the children\n");
        MPI_Recv(arr + left_temp_part[0], left_temp_part[1] - left_temp_part[0] + 1, MPI_INT, NODE_TO_IX(left), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(arr + right_temp_part[0], right_temp_part[1] - right_temp_part[0] + 1, MPI_INT, NODE_TO_IX(right), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        LOG_WITH_RANK("reached the end, sorting...\n");
        qsort(arr + part[0], part[1] - part[0] + 1, sizeof(int), comp);
    }

    if (rank == 0) {
        for (int i = 0; i <= high; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        MPI_Send(arr + part[0], part[1] - part[0] + 1, MPI_INT, NODE_TO_IX(parent), 0, MPI_COMM_WORLD);
    }
    
end:
    MPI_Finalize();
}

static int arr[] = {23, 12, 45, 89, 34, 67, 11, 78, 56, 90, 43, 21, 65, 32, 98, 87};
SAMPLE(quicksort, arr, 0, 15);
