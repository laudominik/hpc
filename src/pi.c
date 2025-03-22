#include <mpi.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG
#include <mpi_samples.h>

#define PRECISION 1000000000

static void pi(int _){
    int rank, world, thread_support;
    int start, end;
    double pi_quarter_local = 0.0;
    double pi_quarter = 0.0;

    MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &thread_support);
    assert(thread_support == MPI_THREAD_MULTIPLE);    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    get_start_end(rank, world, PRECISION, &start, &end);


    #pragma omp parallel for reduction(+:pi_quarter_local)
    for(int i = start; i <= end; i++){
        int sign = i % 2 == 0 ? 1 : -1;
        double denom = sign * (2 * i + 1);
        pi_quarter_local += 1.0/denom;
    }
 
    // share the result with each node
    MPI_Allreduce(&pi_quarter_local, &pi_quarter, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("PI: %.20f \n", 4 * pi_quarter);
    }

    MPI_Finalize();
}
SAMPLE(pi, 0);
