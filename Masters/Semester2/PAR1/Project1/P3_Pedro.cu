#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <opencv2/opencv.hpp>

__global__ void smoothing(float3 *original,
                          float3 *current,
                          float3 *next,
                          float *error,
                          int width,
                          int height,
                          float lambda)
{
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x >= width || y >= height)
    return;

  int idx = y * width + x;

  float3 sum = make_float3(0.f, 0.f, 0.f);
  int count = 0;

  if (x > 0)
  {
    float3 v = current[idx - 1];
    sum.x += v.x;
    sum.y += v.y;
    sum.z += v.z;
    count++;
  }

  if (x < width - 1)
  {
    float3 v = current[idx + 1];
    sum.x += v.x;
    sum.y += v.y;
    sum.z += v.z;
    count++;
  }

  if (y > 0)
  {
    float3 v = current[idx - width];
    sum.x += v.x;
    sum.y += v.y;
    sum.z += v.z;
    count++;
  }

  if (y < height - 1)
  {
    float3 v = current[idx + width];
    sum.x += v.x;
    sum.y += v.y;
    sum.z += v.z;
    count++;
  }

  float3 new_value;

  new_value.x = (original[idx].x + lambda * sum.x) / (1.f + lambda * count);
  new_value.y = (original[idx].y + lambda * sum.y) / (1.f + lambda * count);
  new_value.z = (original[idx].z + lambda * sum.z) / (1.f + lambda * count);

  next[idx] = new_value;

  // L1 error
  error[idx] =
      fabsf(new_value.x - current[idx].x) +
      fabsf(new_value.y - current[idx].y) +
      fabsf(new_value.z - current[idx].z);
}

__global__ void reduce_sum(float *input, float *output, int n)
{
  extern __shared__ float sdata[];

  unsigned int tid = threadIdx.x;
  unsigned int idx = blockIdx.x * blockDim.x * 2 + threadIdx.x;

  float sum = 0.0f;

  if (idx < n)
    sum = input[idx];

  if (idx + blockDim.x < n)
    sum += input[idx + blockDim.x];

  sdata[tid] = sum;
  __syncthreads();

  // Parallel reduction inside block
  for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
  {
    if (tid < s)
      sdata[tid] += sdata[tid + s];

    __syncthreads();
  }

  if (tid == 0)
    output[blockIdx.x] = sdata[0];
}

float gpu_reduce(float *d_input, int size)
{
  int threads = 256;
  int blocks = (size + threads * 2 - 1) / (threads * 2);

  float *d_intermediate;
  cudaMalloc(&d_intermediate, blocks * sizeof(float));

  reduce_sum<<<blocks, threads, threads * sizeof(float)>>>(
      d_input, d_intermediate, size);

  int remaining = blocks;

  while (remaining > 1)
  {
    int new_blocks = (remaining + threads * 2 - 1) / (threads * 2);

    reduce_sum<<<new_blocks, threads, threads * sizeof(float)>>>(
        d_intermediate, d_intermediate, remaining);

    remaining = new_blocks;
  }

  float result;
  cudaMemcpy(&result, d_intermediate,
             sizeof(float), cudaMemcpyDeviceToHost);

  cudaFree(d_intermediate);

  return result;
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
    return 1;
  }

  cv::Mat frame_original = cv::imread(argv[1]);
  if (frame_original.empty())
  {
    printf("Image not loaded\n");
    return 1;
  }

  int width = frame_original.cols;
  int height = frame_original.rows;

  cv::Mat frame_float;
  frame_original.convertTo(frame_float, CV_32FC3, 1.0 / 255.0);

  cv::Mat frame_modified(height, width, CV_32FC3);

  size_t size = width * height * sizeof(float3);

  float3 *d_original, *d_current, *d_next;

  cudaMalloc(&d_original, size);
  cudaMalloc(&d_current, size);
  cudaMalloc(&d_next, size);

  cudaMemcpy(d_original, frame_float.ptr<float3>(), size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_current, frame_float.ptr<float3>(), size, cudaMemcpyHostToDevice);

  dim3 block(16, 16);
  dim3 grid((width + 15) / 16, (height + 15) / 16);

  float lambda = 0.2f;

  int max_iter = 100;
  float tol = 1e-6;

  float *d_error;
  size_t num_pixels = width * height;

  cudaMalloc(&d_error, num_pixels * sizeof(float));

  for (int i = 0; i < max_iter; i++)
  {
    smoothing<<<grid, block>>>(
        d_original, d_current, d_next,
        d_error,
        width, height, lambda);

    cudaDeviceSynchronize();

    float total_error = gpu_reduce(d_error, width * height) / (width * height);

    printf("err: %f ", total_error);
    if (total_error < tol)
    {
      printf("Converged at iteration %d\n", i);
      break;
    }

    std::swap(d_current, d_next);
  }

  cudaMemcpy(frame_modified.ptr<float3>(), d_current, size, cudaMemcpyDeviceToHost);

  frame_modified.convertTo(frame_modified, CV_8UC3, 255.0);
  cv::imwrite(argv[2], frame_modified);

  cudaFree(d_original);
  cudaFree(d_current);
  cudaFree(d_next);
  cudaFree(d_error);

  return 0;
}