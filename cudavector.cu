#include <iostream>
#include <cstdlib>
#include <cuda_runtime.h>

using namespace std;

#define N 100000 // Size of the vector

// CUDA kernel for vector addition
__global__ void vectorAdd(int *A, int *B, int *C, int size) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index < size) {
        C[index] = A[index] + B[index];
    }
}

int main() {
    int *A, *B, *C;  // Host vectors
    int *d_A, *d_B, *d_C;  // Device vectors

    // Allocate memory for host vectors
    A = (int *)malloc(N * sizeof(int));
    B = (int *)malloc(N * sizeof(int));
    C = (int *)malloc(N * sizeof(int));

    // Allocate memory for device vectors
    cudaMalloc(&d_A, N * sizeof(int));
    cudaMalloc(&d_B, N * sizeof(int));
    cudaMalloc(&d_C, N * sizeof(int));

    // Initialize vectors on host
    for (int i = 0; i < N; i++) {
        A[i] = rand() % 100;  // Random values between 0-99
        B[i] = rand() % 100;
    }

    // Copy data from host to device
    cudaMemcpy(d_A, A, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N * sizeof(int), cudaMemcpyHostToDevice);

    // Launch kernel with appropriate block and grid size
    int blockSize = 256;  // Number of threads per block
    int numBlocks = (N + blockSize - 1) / blockSize;  // Number of blocks

    vectorAdd<<<numBlocks, blockSize>>>(d_A, d_B, d_C, N);

    // Copy result from device to host
    cudaMemcpy(C, d_C, N * sizeof(int), cudaMemcpyDeviceToHost);

    // Print result of first 10 elements
    for (int i = 0; i < 10; i++) {
        cout << A[i] << " + " << B[i] << " = " << C[i] << endl;
    }

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Free host memory
    free(A);
    free(B);
    free(C);

    return 0;
}
//!nvcc -arch=sm_70 cudavector.cu -o first
//!./first