#include <mpi.h>
#include <stdio.h>

#include <mpi_samples.h>

static void is_prime(int num){
    int rank, size, end, i;
    int is_prime_global = 1, is_prime_local = 1;
    
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (
        i = 2 + rank * (num - 2) / size, end = i+ num / size;
        i < end && num % i;
        i++
    ); 
    is_prime_local = (i == end);
    MPI_Reduce(&is_prime_local, &is_prime_global, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("%d", is_prime_global);
    }
    MPI_Finalize();
}
SAMPLE(is_prime, 71);
