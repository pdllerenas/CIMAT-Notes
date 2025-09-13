#pragma once
#include "matrix.h"


/**
 * @brief Reads all doubles from a .txt file into a dynamically allocated
 * array.
 *
 * @param filename The path to the .txt file.
 * @param count A pointer to an integer that will be updated with the number of
 * doubles read.
 * @return A pointer to the dynamically allocated array of doubles, or NULL on
 * failure. The caller is responsible for freeing this memory.
 */
double *flat_double_array_from_txt(const char *filename, int *count);

/**
 * @brief Reads all ints from a .txt file into a dynamically allocated
 * array.
 *
 * @param filename The path to the .txt file.
 * @param count A pointer to an integer that will be updated with the number of
 * doubles read.
 * @return A pointer to the dynamically allocated array of ints, or NULL on
 * failure. The caller is responsible for freeing this memory.
 */
int *int_array_from_txt(const char *filename, int *count);

/**
 * @brief Reads all floats from a .txt file into a dynamically allocated
 * array.
 *
 * @param filename The path to the .txt file.
 * @param count A pointer to an integer that will be updated with the number of
 * doubles read.
 * @return A pointer to the dynamically allocated array of floats, or NULL on
 * failure. The caller is responsible for freeing this memory.
 */
float *flat_float_array_from_txt(const char *filename, int *count);

Matrix *load_matrix_from_txt(const char* filename, int* rows, int* cols);

