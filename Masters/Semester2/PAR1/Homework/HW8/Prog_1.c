# include <stdlib.h>
# include <stdio.h>
# include <omp.h>

int main ( int argc, char *argv[] );
int number_1 ( int n );
int number_2 ( int n );
int number_3 ( int n );

/******************************************************************************/

int main ( int argc, char *argv[] )

{
  int n;
  int n_factor;
  int n_hi;
  int n_lo;
  int numbers;
  double time1;
  double time2;
  double time3;

  printf ( "\n" );
  printf ( "  Number of processors available = %d\n", omp_get_num_procs ( )  );
  printf ( "  Number of threads =              %d\n", omp_get_max_threads ( )  );

  n_lo = 1;
  n_hi = 131072;
  n_factor = 2;

  printf ( "\n" );
  printf ( "                           Output #1       Output #2     Output #3\n" );
  printf ( "         N     Pi(N)          Time          Time          Time\n" );
  printf ( "\n" );

  n = n_lo;

  while ( n <= n_hi )
  {
    time1 = omp_get_wtime ( );
    numbers = number_1 ( n );
    time1 = omp_get_wtime ( ) - time1;

    time2 = omp_get_wtime ( );
    numbers = number_2 ( n );
    time2 = omp_get_wtime ( ) - time2;

    time3 = omp_get_wtime ( );
    numbers = number_3 ( n );
    time3 = omp_get_wtime ( ) - time3;

    printf ( "  %8d  %8d  %12f  %12f  %12f\n", n, numbers, time1, time2, time3 );

    n = n * n_factor;
  }

  printf ( "\n" );
  printf ( "  Normal end of execution.\n" );

  return 0;
}
/******************************************************************************/

int number_1( int n )

{
  int i;
  int j;
  int number;
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, number )

# pragma omp for reduction ( + : total )
  for ( i = 2; i <= n; i++ )
  {
    number = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        number = 0;
        break;
      }
    }
    total = total + number;
  }

  return total;
}
/******************************************************************************/

int number_2 ( int n )
{
  int i;
  int j;
  int number;
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, number )

# pragma omp for reduction ( + : total ) schedule ( static, 100 )
  for ( i = 2; i <= n; i++ )
  {
    number = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        number = 0;
        break;
      }
    }
    total = total + number;
  }

  return total;
}
/******************************************************************************/

int number_3 ( int n )


{
  int i;
  int j;
  int number;
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, number )

# pragma omp for reduction ( + : total ) schedule ( dynamic, 100 )
  for ( i = 2; i <= n; i++ )
  {
    number = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        number = 0;
        break;
      }
    }
    total = total + number;
  }

  return total;
}
