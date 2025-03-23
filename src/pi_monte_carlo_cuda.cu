#include <samples.h>
#include <stdio.h>
#include <stdlib.h>
#include <curand_kernel.h>

#define SAMPLES_PER_THREAD 10000
#define THREADS_PER_BLOCK 256
// ceil (N / TPB)
#define BLOCKS_PER_GRID 10000
#define N BLOCKS_PER_GRID * THREADS_PER_BLOCK

__global__ void kernel_pi(unsigned long *results){
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    __shared__ unsigned long shared_blocks[THREADS_PER_BLOCK];
    curandState state;
    curand_init(clock64() * blockIdx.x, idx, 0, &state);

    unsigned long count = 0;
    for (int i = 0; i < SAMPLES_PER_THREAD; i++) {
        double x = curand_uniform(&state) - 3e-6;
        double y = curand_uniform(&state) - 3e-6;
        if (x*x + y*x <= 1) {
            count++;    
        }
    }
    shared_blocks[threadIdx.x] = count;
    __syncthreads();

    if(threadIdx.x == 0) {
        unsigned long count = 0;
        for(int i = 0; i < THREADS_PER_BLOCK; i++) {
            count += shared_blocks[threadIdx.x];
        }
        results[blockIdx.x] = count;
    }
}

int pi(int _){
    unsigned long *h_results; 
    unsigned long *d_results;

    h_results = (unsigned long*) malloc(BLOCKS_PER_GRID * sizeof(unsigned long));
    cudaMalloc(&d_results, BLOCKS_PER_GRID * sizeof(unsigned long));

    kernel_pi<<<BLOCKS_PER_GRID, THREADS_PER_BLOCK>>>(d_results); cudaDeviceSynchronize();
    
    cudaMemcpy(h_results, d_results, BLOCKS_PER_GRID * sizeof(unsigned long), cudaMemcpyDeviceToHost);

    unsigned long sum = 0;
    for(int i = 0; i < BLOCKS_PER_GRID; i++){
        sum += h_results[i];
    }
    double denom = double(N) * double(SAMPLES_PER_THREAD);
    double pi = 4.0 * (double) sum / denom;
    printf("result: %.25f\n", pi);
    
    free(h_results);
    cudaFree(d_results);

    return 0;
}
SAMPLE(pi, 0);
