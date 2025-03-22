#include <mpi.h>
#include <stdio.h>

#include <mpi_samples.h>

#define F(x) (x*x + 3)
#define PRECISION 0.000001

#define DATA 0
#define RESULT 1
#define FINISH 2

#define DEBUG
#undef DEBUG
int (*fptr)(int, int);
typedef double(*integr_f)(double);

static void bubblesort(int a[], int n) {
    int rank;
    int world;
    int phase;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);



    MPI_Finalize();
}

int a[] = {3,2,1,4,5,4,3,2,1};
SAMPLE(bubblesort, a, 3);
