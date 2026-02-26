#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <omp.h>
#include <cstring>
#include <chrono>
using namespace cv;
using namespace std;

void suavizado_openMP(double *f, double *g, int N, int M, double lambda, double tol, int max_iter, int threads)
{
  omp_set_num_threads(threads);
  double *f_ = new double[N * M];

  for (int i = 0; i < max_iter; i++)
  {
    double error = 0.0;

#pragma omp parallel for schedule(static) reduction(+ : error)
    for (int j = 0; j < N; j++)
    {
      for (int k = 0; k < M; k++)
      {
        int idx = M * j + k;

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

        f_[idx] = (g[idx] + lambda * s_vec) / (1.0 + lambda * n_vec);
        error += fabs(f_[idx] - f[idx]);
      }
    }

#pragma omp parallel for
    for (int j = 0; j < N * M; j++)
      f[j] = f_[j];

    if (error < tol)
    {
      printf("Convergencia en %d iteraciones\n", i);
      break;
    }
  }
}

/*
int main(){
    double* img = (double*) malloc(200*300*sizeof(double));
    double* sm = (double*) malloc(200*300*sizeof(double));
    for(int i = 0; i < 200*300; i++){
        img[i] = sqrt(i);
        sm[i] = img[i];
    }

    auto ti = chrono::steady_clock::now();
    suavizado_openMP(img, sm, 200, 300, 0.2, 1e-3, 200, 8);
    auto tf = chrono::steady_clock::now();

    chrono::duration<double> t = tf - ti;

    cout << "Tiempo de ejecución: " << t.count() << endl;

    free(img);
    free(sm);
}
*/

int main()
{
  Mat img = imread("input.webp", IMREAD_COLOR);
  int N = img.rows;
  int M = img.cols;

  vector<Mat> channels;
  split(img, channels);

  double lambda = 0.2;
  double tol = 1e-6;
  int max_iter = 200;
  int threads = 8;

  for (int c = 0; c < 3; c++)
  {
    channels[c].convertTo(channels[c], CV_64F);

    if (!channels[c].isContinuous())
      channels[c] = channels[c].clone();

    double *orig = (double *)channels[c].data;
    double *suave = (double *)malloc(N * M * sizeof(double));
    memcpy(suave, orig, N * M * sizeof(double));

    auto ti = chrono::steady_clock::now();
    suavizado_openMP(suave, orig, N, M, lambda, tol, max_iter, threads);
    auto tf = chrono::steady_clock::now();

    chrono::duration<double> t = tf - ti;

    cout << "Tiempo de ejecución: " << t.count() << endl;

    memcpy(channels[c].data, suave, N * M * sizeof(double));
    free(suave);
    channels[c].convertTo(channels[c], CV_8U);
  }

  merge(channels, img);

  imwrite("output.webp", img);

  return 0;
}
