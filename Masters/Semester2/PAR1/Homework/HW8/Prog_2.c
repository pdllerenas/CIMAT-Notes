# include <omp.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main ( )
{
  double t1;
  double t2;

  printf ( "Output #1\n" );
/*
  Output #1.
*/
  printf ( "\n" );
  printf ( "  omp_get_max_threads ( ) = %d\n", omp_get_max_threads ( ) );
  printf ( "  omp_get_num_threads ( ) = %d\n", omp_get_num_threads ( ) );
  printf ( "  omp_get_thread_num ( ) =  %d\n", omp_get_thread_num ( ) );
  printf ( "  omp_get_num_procs ( )   = %d\n", omp_get_num_procs ( ) );
  printf ( "  omp_get_wtime ( ) =       %g\n", omp_get_wtime ( ) );
  printf ( "\n \n" );
/*
  Output #2:
*/
  omp_set_num_threads ( 5 );

  printf ( "Output #2\n" );
  printf ( "  omp_get_max_threads ( ) = %d\n", omp_get_max_threads ( ) );
  printf ( "  omp_get_num_threads ( ) = %d\n", omp_get_num_threads ( ) );
  printf ( "  omp_get_thread_num ( ) =  %d\n", omp_get_thread_num ( ) );
  printf ( "  omp_get_num_procs ( )   = %d\n", omp_get_num_procs ( ) );
  printf ( "  omp_get_wtime ( ) =       %g\n", omp_get_wtime ( ) );
  printf ( "\n \n" );
/*
  Output #3:
*/
  omp_set_num_threads ( 3 );
  printf ( "Output #3\n" );
# pragma omp parallel
{
  printf ( "  omp_get_thread_num ( ) =  %d\n", omp_get_thread_num ( ) );

# pragma omp single
  {
    printf ( "\n" );
    printf ( "  omp_get_max_threads ( ) = %d\n", omp_get_max_threads ( ) );
    printf ( "  omp_get_num_threads ( ) = %d\n", omp_get_num_threads ( ) );
    printf ( "  omp_get_thread_num ( ) =  %d\n", omp_get_thread_num ( ) );
    printf ( "  omp_get_num_procs ( )   = %d\n", omp_get_num_procs ( ) );
    printf ( "  omp_get_wtime ( ) =       %g\n", omp_get_wtime ( ) );
    printf ( "\n \n" );
  }
}

    printf ( "\n \n" );
/*
  Output #4:
*/
  printf ( "Output #4\n" );
  t1 = omp_get_wtime ( );
  sleep ( 5 );
  t2 = omp_get_wtime ( );
  printf ( "  omp_get_wtime estimates duration of sleep(5) as %g seconds.\n", t2 - t1 );
/*
  Terminate.
*/
  printf ( "\n" );
  printf ( "  Normal end of execution.\n" );

  return 0;
}

