#pragma once

#include <stddef.h>

typedef struct {
  int dim;
  size_t type;
  double *data;
} Vector;

Vector *create_vector(int);
double dot(const Vector *, const Vector *);
int vector_compare(const Vector *, const Vector *, double);
double vector_norm_squared(const Vector *);
void vector_axpy_inplace(Vector *x, double proj, Vector *prev);
void vmadd(Vector *a, Vector *b, Vector *c, double s);

Vector *vector_sum(const Vector *, const Vector *);
void vector_sum_inplace(Vector *, const Vector *);
Vector *vector_diff(const Vector *, const Vector *);
void vector_diff_inplace(Vector *, const Vector *);
Vector *load_vector_from_txt(const char *);
Vector *vector_fill(int, double);
Vector *vdiv(Vector *x, double d);
void vector_swap_rows(Vector *, int, int);
void copy_data(Vector *, const Vector *);
void print_vector(Vector *);
void print_tex_table(Vector *);
void free_vector(Vector *);
int vector_arg_max(const Vector *);
Vector* vector_scalar_product(const Vector*, const double);
void vector_scalar_product_inplace(Vector*, const double);
double max_norm(const Vector *);
double l2_norm(const Vector *);
