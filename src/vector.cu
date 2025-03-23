#include <samples.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000
#define THREADS_PER_BLOCK 256
// ceil (N / TPB)
#define BLOCKS_PER_GRID (N + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK 

__global__ void vadd(float *A, float *B, float *C){
    int ix = threadIdx.x + blockIdx.x * blockDim.x;
    if (ix >= N) {
        return;
    }
    C[ix] = A[ix] + B[ix];
}

int vector_add(int _){

    // host
    float *h_A, *h_B, *h_C; 
    float *d_A, *d_B, *d_C;  // device 
    srand(time(NULL));

    h_A = (float*) malloc(N * sizeof(float));
    h_B = (float*) malloc(N * sizeof(float));
    h_C = (float*) malloc(N * sizeof(float));

    cudaMalloc(&d_A, N * sizeof(float));
    cudaMalloc(&d_B, N * sizeof(float));
    cudaMalloc(&d_C, N * sizeof(float));

    /* generate data */
    for (int i = 0; i < N; i++){
        h_A[i] = (float) rand() / (float) RAND_MAX;
        h_B[i] = (float) rand() / (float) RAND_MAX;
    }

    cudaMemcpy(d_A, h_A, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, N * sizeof(float), cudaMemcpyHostToDevice);

    vadd<<<BLOCKS_PER_GRID, THREADS_PER_BLOCK>>>(d_A, d_B, d_C); cudaDeviceSynchronize();
    cudaMemcpy(h_C, d_C, N * sizeof(float), cudaMemcpyDeviceToHost);

    for (int i = 0; i < 10; i++) {
        printf("%f ", h_A[i]);
    }
    printf("\n");

    for (int i = 0; i < 10; i++) {
        printf("%f ", h_B[i]);
    }
    printf("\n");

    for (int i = 0; i < 10; i++) {
        printf("%f ", h_C[i]);
    }
    printf("\n");
    
    free(h_A); free(h_B); free(h_C);
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);

    return 0;
}
SAMPLE(vector_add, 0);
