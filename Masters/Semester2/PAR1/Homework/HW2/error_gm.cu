#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>

constexpr int BLOCK_SIZE = 4;

int div_up(int n, int d) {
  if (n < 0) {
    return -(-n / d);
  } else {
    return (n + d - 1) / d;
  }
}

__global__ void MatrixSquaredError_GM(float *E, float *A, float *B, int n_row, int n_col){
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int idy = blockIdx.y * blockDim.y + threadIdx.y;

  *E = 0.0f;
  if (idx < n_col && idy < n_row) {
    int index = idy * n_col + idx;
    *E += (A[index] - B[index]) * (A[index] - B[index]);
  }
}

int main(void) {
  float *A_h, *B_h, *C_h;
  float *A_d, *B_d, *C_d;
  int n_row, n_col;
  printf("Enter number of rows and columns: ");
  scanf("%d %d", &n_row, &n_col);

  int N = n_row * n_col;
  cudaEvent_t start, stop;
  float time;

  size_t size = N * sizeof(float);
  A_h = (float *)malloc(size);
  B_h = (float *)malloc(size);
  C_h = (float *)malloc(sizeof(float));

  for (int i = 0; i < n_row; i++) {
    for (int j = 0; j < n_col; j++) {
      A_h[i*n_col+j] = rand() / (float)RAND_MAX;
      B_h[i*n_col+j] = rand() / (float)RAND_MAX;
    }
  }

  cudaMalloc((void **)&A_d, size);
  cudaMalloc((void **)&B_d, size);
  cudaMalloc((void **)&C_d, size);

  cudaMemcpy(A_d, A_h, size, cudaMemcpyHostToDevice);
  cudaMemcpy(B_d, B_h, size, cudaMemcpyHostToDevice);

  dim3 block_size(BLOCK_SIZE, BLOCK_SIZE);
  dim3 n_blocks(div_up(n_col, block_size.x), div_up(n_row, block_size.y));

  MatrixSquaredError_GM<<<n_blocks, block_size>>>(C_d, A_d, B_d, n_row, n_col);
  cudaMemcpy(C_h, C_d, sizeof(float), cudaMemcpyDeviceToHost);
  printf("Squared Error: %f\n", C_h);

  free(A_h);
  free(B_h);
  free(C_h);
  cudaFree(A_d);
  cudaFree(B_d);
  cudaFree(C_d);

}