#include <mpi.h>
#include <omp.h>
#include <stdio.h>

#define DEBUG
#include <omp_samples.h>

static void hello(int _) {
    omp_set_num_threads(4);
    # pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        printf("Hello!\n");
        #pragma omp master 
        {
            printf("Master!\n");
        }

        #pragma omp critical
        {
            for(int i = 0; i < 3; i++){
                LOG_WITH_THREADID("%d \n", i);
            }
        }
       
    }
}
SAMPLE(hello, 0)
