#include <cuda_runtime.h>

constexpr int BLOCK_SIZE = 4;

__global__ void MatrixSquaredError(float *C, float *A, float *B, int n_row, int n_col){
  int bx = blockIdx.x;
  int by = blockIdx.y;

  int tx = threadIdx.x;
  int ty = threadIdx.y;

  int aBegin = n_col * BLOCK_SIZE * by;
  int aEnd   = aBegin + n_col - 1;

  int aStep = BLOCK_SIZE; 

  int bBegin = BLOCK_SIZE * bx;
  int bStep = BLOCK_SIZE * n_col;

  float sum_sub = 0.0f;

  for (int a = aBegin, b = bBegin; a <= aEnd; a+= aStep, b+= bStep) {
    __shared__ float As[BLOCK_SIZE][BLOCK_SIZE];
    __shared__ float Bs[BLOCK_SIZE][BLOCK_SIZE];

    As[ty][tx] = A[a+n_col*ty + tx];
    Bs[ty][tx] = B[b+n_col*ty + tx];

    __syncthreads();
  }

  int c = n_col * BLOCK_SIZE * by + BLOCK_SIZE * bx;
  C[c + n_col * ty + tx] = sum_sub;
}