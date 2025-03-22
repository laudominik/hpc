#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi_samples.h>

#define F(x) (x*x + 3)
#define PRECISION 0.00001

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

/* 
    this is almost the same as integrate.c, but with a twist
    all of the requests/responses are sent non-blocking so it's faster
    but we need to store it somewhere so it's less readable
*/

static void integrate(double a, double b, integr_f f) {
    int rank;
    int world;
    double area = 0.0;
    double temp_area;
    double current_a;
    int workers;
    MPI_Status status;
    double range[2];
    MPI_Request* requests;
    double* ranges;
    double* resulttemp;
    int request_completed;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    workers = world - 1;

    if (rank == world - 1) {
        // master
        current_a = a;
        requests=(MPI_Request*)malloc(3*workers*sizeof(MPI_Request));
        ranges = (double *)malloc(4*workers*sizeof(double));
        resulttemp=(double *)malloc(workers*sizeof(double));
        
        for (int i = 0; i < 3 * workers; i++) {
            requests[i] = MPI_REQUEST_NULL;
        }

        /* master logic:
            1. send N requests (nonblocking)
            2. receive N responses (nonblocking)
            3. for each response received send back a new request until entire range is covered
            4. wait for all the remaining requests
        */

        for(int i = 0; i < workers; i++) {
            range[0] = current_a;
            current_a += PRECISION;
            range[1] = current_a;

            ranges[2 * i] = range[0];
            ranges[2 * i+1] = range[1];

            MPI_Isend(ranges + 2 * i, 2, MPI_DOUBLE, i, DATA, MPI_COMM_WORLD, requests + workers + i);
        }

        for(int i = 0; i < workers; i++) {
            MPI_Irecv(resulttemp + i, 1, MPI_DOUBLE, i, RESULT, MPI_COMM_WORLD, requests + i);
        }

        while (current_a < b) {
            MPI_Waitany(2 * workers, requests, &request_completed, MPI_STATUS_IGNORE);

            if (request_completed >= workers || request_completed < 0) {
                // this just means SEND was completed
                // or nothing was received (busy loop for some reason...)
                continue;
            }
            // integration result
            area += resulttemp[request_completed];
            MPI_Wait(&(requests[workers+request_completed]), MPI_STATUS_IGNORE);
            
            range[0] = current_a;
            current_a += PRECISION;
            range[1] = current_a;
            ranges[2 * request_completed] = range[0];
            ranges[2 * request_completed+1] = range[1];
            MPI_Isend(ranges + 2 * request_completed, 2, MPI_DOUBLE, request_completed, DATA, MPI_COMM_WORLD, requests + workers + request_completed);
            MPI_Irecv(resulttemp + request_completed, 1, MPI_DOUBLE, request_completed, RESULT, MPI_COMM_WORLD, requests + request_completed);
        }

        for (int i = 0; i < workers; i++) {
            int nothing;
            MPI_Isend(&nothing, 1, MPI_INT, i, FINISH, MPI_COMM_WORLD, requests + 2*workers + i);
        }

        MPI_Waitall (3*workers, requests, MPI_STATUSES_IGNORE);

        for (int i = 0; i < workers; i++) {
            area += resulttemp[i];
        }
        printf("AUC:  %f range: [%f %f] \n", area, a, b);

        free(requests);
        free(ranges);
        free(resulttemp);
    } else {
        // slave
        MPI_Request send_request = MPI_REQUEST_NULL;
    
        while (1) {
            MPI_Probe(world - 1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if (status.MPI_TAG == FINISH) {
                break;
            }             
            MPI_Recv(range, 2, MPI_DOUBLE, world - 1, DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Computing for range [%f, %f] \n", range[0], range[1]);
            area = (range[1] - range[0]) * f(range[0]);
            MPI_Wait(&send_request, MPI_STATUS_IGNORE);
            MPI_Isend(&area, 1, MPI_DOUBLE, world-1, RESULT, MPI_COMM_WORLD, &send_request);
        }
    }
    
    MPI_Finalize();

}

SAMPLE(integrate, 0, 3, cos);
