#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"

int *SortFile(char *filename, void (*sort)(int *A, int N))
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("Could not open file");
    return NULL;
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int k = 0;

  // read header
  read = getline(&line, &len, fp);
  if (read == -1)
  {
    fprintf(stderr, "Could not read file");
    return NULL;
  }
  int size;
  if (sscanf(line, "SIZE %d", &size) != 1)
  {
    fprintf(stderr, "Invalid header format\n");
    free(line);
    fclose(fp);
    return NULL;
  }

  int *arr = (int *)malloc(size * sizeof(int));

  while ((read = getline(&line, &len, fp)) != -1 && k < size)
  {
    arr[k++] = atoi(line);
  }
  sort(arr, size);

  free(line);
  fclose(fp);

  return arr;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    fprintf(stderr, "Invalid argument count. Usage: %s <count> <max_int> <out_file>\n", argv[0]);
    return -1;
  }
  srand(time(NULL));
  int N = atoi(argv[1]);
  if (N <= 0)
  {
    fprintf(stderr, "Invalid count.");
    return -1;
  }

  int M = atoi(argv[2]);
  if (M <= 0)
  {
    fprintf(stderr, "Invalid max int.");
    return -1;
  }

  char *filename = argv[3];
  FILE *fp = fopen(filename, "w");
  if (fp == NULL)
  {
    perror("Error opening file for writing");
    return -1;
  }

  // header for size
  fprintf(fp, "SIZE %d\n", N);
  for (int i = 0; i < N; i++)
  {
    fprintf(fp, "%d\n", (rand() % M));
  }

  fclose(fp);

  clock_t t0, tf;
  double cpu_time_used;

  // ========= Bubble sort ============
  t0 = clock();
  int *arr1 = SortFile(filename, BubbleSort);
  tf = clock();
  cpu_time_used = ((double)(tf - t0)) / CLOCKS_PER_SEC;
  printf("Bubble sort time: %lf s\n", cpu_time_used);
  // ========= Bubble sort ============

  // ========= OddEven sort ============
  t0 = clock();
  int *arr2 = SortFile(filename, OddEvenSort);
  tf = clock();

  cpu_time_used = ((double)(tf - t0)) / CLOCKS_PER_SEC;
  printf("OddEven sort time: %lf s\n", cpu_time_used);
  // ========= OddEven sort ============

  // ========= Insertion sort ============
  t0 = clock();
  int *arr3 = SortFile(filename, InsertionSort);
  tf = clock();

  cpu_time_used = ((double)(tf - t0)) / CLOCKS_PER_SEC;
  printf("Insertion sort time: %lf s\n", cpu_time_used);
  // ========= Insertion sort ============

  free(arr1);
  free(arr2);
  free(arr3);

  return 0;
}