#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "utils/conjugate_gradient.h"
#define TOL 1e-5

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    fprintf(stderr, "Use: %s <file_name.mat> <output.mat> <threads>\n", argv[0]);
    return 1;
  }
  omp_set_num_threads(atoi(argv[3]));
  printf("Number of threads set: %d\n", omp_get_max_threads());

  FILE *file = fopen(argv[1], "rb");
  if (!file)
  {
    fprintf(stderr, "Could not open input file %s\n", argv[1]);
    return 1;
  }

  int header[5];
  size_t read = fread(header, sizeof(int), 5, file);
  if (read != 5)
    return 1;

  int format = header[0] % 10;
  int type = header[0] - format;

  printf("format: %i (%s)\n", format, (format == 0) ? "full matrix" : (format == 1) ? "text"
                                                                  : (format == 2)   ? "Sparse matrix"
                                                                                    : "INVALID");
  printf("type:   %i (%s)\n", type, (type == 0) ? "double" : (type == 10) ? "float"
                                                         : (type == 20)   ? "int"
                                                         : (type == 30)   ? "short"
                                                         : (type == 40)   ? "unsigned short"
                                                         : (type == 50)   ? "unsigned char"
                                                                          : "INVALID");

  if (format == 0) // Full matrix
  {
    int rows = header[1];
    int cols = header[2];
    int field = header[3];
    int name_size = header[4];
    char name[128];
    read = fread(name, name_size, 1, file);

    printf("rows:  %i\n", rows);
    printf("cols:  %i\n", cols);
    printf("field: %i (%s)\n", field, (field == 0) ? "real" : (field == 1) ? "complex"
                                                                           : "INVALID");
    printf("name:  %s\n", name);

    double *tmp = (double *)malloc(rows * cols * sizeof(double));
    read = fread(tmp, sizeof(double), rows * cols, file);

    double *matrix = (double *)malloc(rows * cols * sizeof(double));

    // fill matrix, convert to row major
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        matrix[i * cols + j] = tmp[j * rows + i];
      }
    }

    free(tmp);

    double *b_vector = NULL;
    double *x_vector = NULL;
    int b_size = 0;

    // search for b and x
    for (int step = 0; step < 2; step++)
    {
      int current_header[5];
      if (fread(current_header, sizeof(int), 5, file) == 5)
      {
        int var_rows = current_header[1];
        int var_cols = current_header[2];
        int var_size = var_rows * var_cols;
        int var_name_size = current_header[4];

        char var_name[128];
        fread(var_name, var_name_size, 1, file);

        // printf("Read variable from file: '%s'\n", var_name);

        double *temp_vector = (double *)malloc(var_size * sizeof(double));
        fread(temp_vector, sizeof(double), var_size, file);

        if (strcmp(var_name, "b") == 0)
        {
          b_vector = temp_vector;
          b_size = var_size;
        }
        else if (strcmp(var_name, "x") == 0)
        {
          x_vector = temp_vector;
        }
        else
        {
          free(temp_vector);
        }
      }
    }

    if (b_vector != NULL && x_vector != NULL)
    {
      // intial guess is x0 = 0
      double *x0 = (double *)calloc(b_size, sizeof(double));

// run cg
#ifdef _OPENMP
      double start_time = omp_get_wtime();
#endif
      x0 = conjugate_gradient(matrix, b_vector, x0, rows, TOL, cols);
#ifdef _OPENMP
      double end_time = omp_get_wtime();
      double elapsed_time = end_time - start_time;
#endif

      double l2_error_sq = 0.0;
      double linf_error = 0.0;

      for (int i = 0; i < b_size; i++)
      {
        double diff = x0[i] - x_vector[i];
        if (diff < 0)
          diff = -diff; // Absolute value

        l2_error_sq += diff * diff;

        if (diff > linf_error)
        {
          linf_error = diff;
        }
      }
      double l2_error = sqrt(l2_error_sq);

      printf("\n--- Results ---\n");
      printf("L2 Error Norm:       %e\n", l2_error);
      printf("L_infinity Error Norm: %e\n", linf_error);
#ifdef _OPENMP
      printf("Time: %lf s\n", elapsed_time);
#endif
      printf("--------------------------\n");

      // write vector to file
      FILE *fout = fopen(argv[2], "wb");
      if (fout)
      {
        int out_header[5];
        out_header[0] = 0;
        out_header[1] = b_size;
        out_header[2] = 1;
        out_header[3] = 0;

        const char *out_name = "computed_x";
        out_header[4] = strlen(out_name) + 1;

        fwrite(out_header, sizeof(int), 5, fout);
        fwrite(out_name, sizeof(char), out_header[4], fout);
        fwrite(x0, sizeof(double), b_size, fout);
        fclose(fout);
      }
      else
      {
        fprintf(stderr, "Error creating output file.\n");
      }

      free(x0);
      free(b_vector);
      free(x_vector);
    }
    else
    {
      fprintf(stderr, "Error: Could not find both 'b' and 'x' variables in the file.\n");
      if (b_vector)
        free(b_vector);
      if (x_vector)
        free(x_vector);
    }
  }
  else if (format == 1)
  {
    // Text matrix (Not used)
  }
  else if (format == 2)
  {
    int stored_rows = header[1]; // nnz
    int stored_cols = header[2]; // complex or real
    int name_size = header[4];

    char name[128];
    read = fread(name, name_size, 1, file);
    printf("Found sparse matrix: %s\n", name);

    int total_elements = stored_rows * stored_cols;
    double *raw_sparse_data = (double *)malloc(total_elements * sizeof(double));
    read = fread(raw_sparse_data, sizeof(double), total_elements, file);

    int nnz = stored_rows - 1;

    int actual_rows = (int)raw_sparse_data[nnz];
    int actual_cols = (int)raw_sparse_data[stored_rows + nnz];
    // printf("True Matrix Dimensions: %d x %d\n", actual_rows, actual_cols);
    // printf("NNZ: %d\n", nnz);

    int *row_indices = (int *)malloc(nnz * sizeof(int));
    int *col_indices = (int *)malloc(nnz * sizeof(int));
    double *values = (double *)malloc(nnz * sizeof(double));

    for (int i = 0; i < nnz; i++)
    {
      row_indices[i] = (int)raw_sparse_data[i] - 1;
      col_indices[i] = (int)raw_sparse_data[stored_rows + i] - 1;
      values[i] = raw_sparse_data[2 * stored_rows + i];
    }

    double *b_vector = NULL;
    double *x_vector = NULL;
    int b_size = 0;

    // search for b and x
    for (int step = 0; step < 2; step++)
    {
      int current_header[5];
      if (fread(current_header, sizeof(int), 5, file) == 5)
      {
        int var_rows = current_header[1];
        int var_cols = current_header[2];
        int var_size = var_rows * var_cols;
        int var_name_size = current_header[4];

        char var_name[128];
        fread(var_name, var_name_size, 1, file);

        // printf("Read variable from file: '%s'\n", var_name);

        double *temp_vector = (double *)malloc(var_size * sizeof(double));
        fread(temp_vector, sizeof(double), var_size, file);

        if (strcmp(var_name, "b") == 0)
        {
          b_vector = temp_vector;
          b_size = var_size;
        }
        else if (strcmp(var_name, "x") == 0)
        {
          x_vector = temp_vector;
        }
        else
        {
          free(temp_vector);
        }
      }
    }

    if (b_vector != NULL && x_vector != NULL)
    {
      // intial guess is x0 = 0
      double *x0 = (double *)calloc(b_size, sizeof(double));

// run cg
#ifdef _OPENMP
      double start_time = omp_get_wtime();
#endif
      x0 = conjugate_gradient_sparse(row_indices, col_indices, values, nnz, actual_rows, x0, b_vector, TOL, actual_cols);
#ifdef _OPENMP
      double end_time = omp_get_wtime();
      double elapsed_time = end_time - start_time;
#endif

      double l2_error_sq = 0.0;
      double linf_error = 0.0;

      for (int i = 0; i < b_size; i++)
      {
        double diff = x0[i] - x_vector[i];
        if (diff < 0)
          diff = -diff; // Absolute value

        l2_error_sq += diff * diff;

        if (diff > linf_error)
        {
          linf_error = diff;
        }
      }
      double l2_error = sqrt(l2_error_sq);

      printf("\n--- Results ---\n");
      printf("L2 Error Norm:       %e\n", l2_error);
      printf("L_infinity Error Norm: %e\n", linf_error);
#ifdef _OPENMP
      printf("Time: %lf s\n", elapsed_time);
#endif
      printf("--------------------------\n");

      // write vector to file
      FILE *fout = fopen(argv[2], "wb");
      if (fout)
      {
        int out_header[5];
        out_header[0] = 0;
        out_header[1] = b_size;
        out_header[2] = 1;
        out_header[3] = 0;

        const char *out_name = "computed_x";
        out_header[4] = strlen(out_name) + 1;

        fwrite(out_header, sizeof(int), 5, fout);
        fwrite(out_name, sizeof(char), out_header[4], fout);
        fwrite(x0, sizeof(double), b_size, fout);
        fclose(fout);
      }
      else
      {
        fprintf(stderr, "Error creating output file.\n");
      }

      free(x0);
      free(b_vector);
      free(x_vector);
    }
    else
    {
      fprintf(stderr, "Error: Could not find both 'b' and 'x' variables in the file.\n");
      if (b_vector)
        free(b_vector);
      if (x_vector)
        free(x_vector);
    }

    free(row_indices);
    free(col_indices);
    free(values);
    free(raw_sparse_data);
  }

  fclose(file);
  return 0;
}