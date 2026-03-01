#include <bits/stdc++.h>
#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstring>
#include <chrono>

using namespace cv;
using namespace std;

#define BX 16
#define BY 16

__global__ void suavizado_cuda(const double *f,
                               const double *g,
                               double *f_out,
                               double *err_block,
                               int N, int M,
                               double lambda)
{
  __shared__ double err_s[BY * BX];

  int tx = threadIdx.x;
  int ty = threadIdx.y;
  int j = blockIdx.y * BY + ty;
  int k = blockIdx.x * BX + tx;
  int idx = j * M + k;

  double err = 0.0;

  if (j < N && k < M)
  {
    int n_vec = 0;
    double s_vec = 0.0;

    if (j > 0)
    {
      s_vec += f[idx - M];
      n_vec++;
    }
    if (j < N - 1)
    {
      s_vec += f[idx + M];
      n_vec++;
    }
    if (k > 0)
    {
      s_vec += f[idx - 1];
      n_vec++;
    }
    if (k < M - 1)
    {
      s_vec += f[idx + 1];
      n_vec++;
    }

    double val = (g[idx] + lambda * s_vec) / (1.0 + lambda * n_vec);
    f_out[idx] = val;
    err = fabs(val - f[idx]);
  }

  int tid = ty * BX + tx;
  err_s[tid] = err;
  __syncthreads();

  for (int s = (BX * BY) / 2; s > 0; s /= 2)
  {
    if (tid < s)
      err_s[tid] += err_s[tid + s];
    __syncthreads();
  }

  if (tid == 0)
  {
    atomicAdd(err_block, err_s[0]);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    fprintf(stderr, "Usage: %s <input> <output> <lambda> <max_iter>\n", argv[0]);
    return 1;
  }
  Mat img = imread(argv[1], IMREAD_COLOR);
  int N = img.rows;
  int M = img.cols;

  vector<Mat> channels;
  split(img, channels);

  double lambda = atof(argv[3]);
  double tol = 1e-6;
  int max_iter = atoi(argv[4]);
  double total_time = 0.0;

  for (int c = 0; c < 3; c++)
  {
    channels[c].convertTo(channels[c], CV_64F);

    if (!channels[c].isContinuous())
      channels[c] = channels[c].clone();

    double *orig = (double *)channels[c].data;
    double *suave = (double *)malloc(N * M * sizeof(double));
    memcpy(suave, orig, N * M * sizeof(double));

    double *f_d, *f_new_d, *g_d;
    double *error_d;

    cudaMalloc(&f_d, N * M * sizeof(double));
    cudaMalloc(&f_new_d, N * M * sizeof(double));
    cudaMalloc(&g_d, N * M * sizeof(double));
    cudaMalloc(&error_d, sizeof(double));

    cudaMemcpy(f_d, suave, N * M * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(g_d, orig, N * M * sizeof(double), cudaMemcpyHostToDevice);

    dim3 block(BX, BY);
    dim3 grid((M + BX - 1) / BX,
              (N + BY - 1) / BY);

    double error_h;

    auto ti = chrono::steady_clock::now();

    for (int iter = 0; iter < max_iter; iter++)
    {
      cudaMemset(error_d, 0, sizeof(double));

      suavizado_cuda<<<grid, block>>>(f_d, g_d, f_new_d, error_d, N, M, lambda);

      cudaDeviceSynchronize();
      cudaMemcpy(&error_h, error_d, sizeof(double), cudaMemcpyDeviceToHost);

      if (error_h  < tol )
      {
        cout << "Convergencia en " << iter << " iteraciones\n";
        break;
      }

      swap(f_d, f_new_d);
    }

    cudaMemcpy(suave, f_d, N * M * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(f_d);
    cudaFree(f_new_d);
    cudaFree(g_d);
    cudaFree(error_d);

    auto tf = chrono::steady_clock::now();
    chrono::duration<double> t = tf - ti;
    double time = t.count();
    total_time += time;
    cout << "Tiempo de ejecución: " << time << endl;

    memcpy(channels[c].data, suave, N * M * sizeof(double));
    free(suave);

    channels[c].convertTo(channels[c], CV_8U);
  }
  cout << "Tiempo total de ejecución: " << total_time << endl;
  merge(channels, img);
  imwrite(argv[2], img);

  return 0;
}
