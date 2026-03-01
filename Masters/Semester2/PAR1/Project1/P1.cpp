#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstring>
#include <chrono>
using namespace cv;
using namespace std;

void suavizado_serie(double *f, double *g, int N, int M, double lambda, double tol, int max_iter)
{
  double *f_ = new double[N * M];

  for (int i = 0; i < max_iter; i++)
  {
    double error = 0.0;
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

    for (int j = 0; j < N * M; j++)
      f[j] = f_[j];
    if (error < tol)
    {
      printf("Convergencia en %d iteraciones\n", i);
      break;
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 5)
  {
    fprintf(stderr, "Usage: %s <input> <output> <lambda> <max_iterations>\n", argv[0]);
    return 1;
  }
  Mat img = imread(argv[1], 1);
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

    auto ti = chrono::steady_clock::now();
    suavizado_serie(suave, orig, N, M, lambda, tol, max_iter);
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
