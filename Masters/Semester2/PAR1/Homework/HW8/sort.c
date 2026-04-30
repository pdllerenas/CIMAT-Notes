#include "sort.h"

void BubbleSort(int *A, int N)
{
  int i, j;
  int temp;
  for (i = N - 1; i > 0; i--)
  {
    for (j = 0; j < i; j++)
    {
      if (A[j] > A[j + 1])
      {
        temp = A[j];
        A[j] = A[j + 1];
        A[j + 1] = temp;
      }
    }
  }
}

void OddEvenSort(int *A, int N)
{
  int exch = 1, start = 0, i;
  int temp;
  while (exch || start)
  {
    exch = 0;
    for (i = start; i < N - 1; i += 2)
    {
      if (A[i] > A[i + 1])
      {
        temp = A[i];
        A[i] = A[i + 1];
        A[i + 1] = temp;
        exch = 1;
      }
    }
    if (start == 0)
      start = 1;
    else
      start = 0;
  }
}

void InsertionSort(int *A, int N)
{
  int i, j, v;
  for (i = 1; i < N; i++)
  {
    v = A[i];
    j = i;
    while (A[j - 1] > v)
    {
      A[j] = A[j - 1];
      j--;
      if (j <= 0)
        break;
    }
    A[j] = v;
  }
}