#include <samples.h>
#include <stdio.h>

__global__ void kernel()
{
    printf("Hello from the GPU! Thread ID: %d\n", threadIdx.x);
}

int hello(int _){
    kernel<<<1, 10>>>();
    cudaDeviceSynchronize();
    return 0;
}
SAMPLE(hello, 0);
