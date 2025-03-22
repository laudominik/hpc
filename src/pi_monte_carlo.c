#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG
#include <mpi_samples.h>

#define N_POINTS 100000000

static void pi(int _){
    int rank, world, thread_support;
    int start, end;
    int points_in_circle = 0;
    int points_in_circle_global = 0;
    unsigned int seed = time(NULL);

    MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &thread_support);
    assert(thread_support == MPI_THREAD_MULTIPLE);    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    #pragma omp parallel for reduction(+: points_in_circle)
    for(int i = 0; i < N_POINTS; i++){
        double x = (double) rand_r(&seed) / (double) RAND_MAX;
        double y = (double) rand_r(&seed) / (double) RAND_MAX;
        points_in_circle += x*x + y*y <= 1 ? 1 : 0;
    }
   
    MPI_Reduce(
        &points_in_circle, &points_in_circle_global, 
        1, MPI_INT, MPI_SUM, 
        0, MPI_COMM_WORLD
    );

    if (rank == 0) {
        double pi = 4.0 * (double) points_in_circle_global / (double) (N_POINTS * world);
        printf("PI: %.20f \n", pi);
    }

    MPI_Finalize();
}
SAMPLE(pi, 0);
