#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    double mytime;
    MPI_Init(&argc, &argv);
    mytime = MPI_Wtime();
    int numtasks;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    int taskid;
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    char nodename[MPI_MAX_PROCESSOR_NAME + 1];
    int len;
    MPI_Get_processor_name(nodename, &len);
    cout << "Hello Pedro (C++)! desde el proceso " << taskid << " de " << numtasks << ". Se ejecuta en " << nodename << "\n";
    mytime = MPI_Wtime() - mytime;
    mytime *= 1000;
    cout << "Timing from node " << nodename << " is " << mytime << " ms.\n";
    MPI_Finalize();
}