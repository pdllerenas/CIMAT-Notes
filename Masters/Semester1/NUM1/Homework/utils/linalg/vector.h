#pragma once

#include <stddef.h>

typedef struct {
  int dim;
  size_t type;
  void *data;
} Vector;

Vector *create_vector(int);
double dot(const Vector *, const Vector *);
int vector_compare(const Vector *, const Vector *, double);
double vector_norm_squared(const Vector *);

Vector *vector_sum(const Vector *, const Vector *);
Vector *vector_diff(const Vector *, const Vector *);
Vector *load_vector_from_txt(const char *);
Vector *vector_fill(int, double);
void vector_swap_rows(Vector *, int, int);
void copy_data(Vector *, const Vector *);
void print_vector(Vector *);
void free_vector(Vector *);
int vector_arg_max(const Vector *);
Vector* vector_scalar_product(const Vector*, const double);
void vector_scalar_product_inplace(Vector*, const double);
double max_norm(const Vector *);
double l2_norm(const Vector *);
