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

double f1(double x) {
    return x * x + 3;
}

double f2(double x) {
    return x;
}

static void integrate(double a, double b, integr_f f) {
    int rank;
    int world;
    double area = 0.0;
    double temp_area;
    double range[2];
    double current_a;
    int workers;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    workers = world - 1;

    if (rank == world - 1) {
        // master
        current_a = a;
        for (int i = 0; i < workers; i++) {
            range[0] = current_a; range[1] = range[0] + PRECISION;
            current_a += PRECISION;
            MPI_Send(range, 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

        while (current_a < b) {
            MPI_Recv(&temp_area, 1, MPI_DOUBLE, MPI_ANY_SOURCE, RESULT, MPI_COMM_WORLD, &status);
            
#ifdef DEBUG
            printf("MASTER: Received result from %d = %f\n", status.MPI_SOURCE, temp_area);
#endif 
            area += temp_area;
            
            range[0] = current_a; range[1] = range[0] + PRECISION;
            current_a += PRECISION;
            MPI_Send(range, 2, MPI_DOUBLE, status.MPI_SOURCE, DATA, MPI_COMM_WORLD);
        }

        for (int i = 0; i < workers; i++) {
            MPI_Recv(&temp_area, 1, MPI_DOUBLE, MPI_ANY_SOURCE, RESULT, MPI_COMM_WORLD, &status);
            area += temp_area;
            MPI_Send(&area, 1, MPI_DOUBLE, status.MPI_SOURCE, FINISH, MPI_COMM_WORLD);
        }

        printf("AUC:  %f range: [%f %f] \n", area, a, b);
    } else {
        // slave
        while (1) {
            MPI_Probe(world - 1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if (status.MPI_TAG == FINISH) {
                break;
            }             
            MPI_Recv(range, 2, MPI_DOUBLE, world - 1, DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef DEBUG
                printf("Computing for range [%f, %f] \n", range[0], range[1]);
#endif
            area = (range[1] - range[0]) * f(range[0]);
            MPI_Send(&area, 1, MPI_DOUBLE, world-1, RESULT, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();

}

SAMPLE(integrate, 0, 3, f2);
