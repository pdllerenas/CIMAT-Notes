#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <mpi.h>

double T(double x, double y)
{
	return 20.0 * (std::sin(4.0 * x) * std::sin(3.0 * y) + 0.3 * std::cos(6.0 * x) * std::sin(5.0 * y) + 1.0);
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int taskid, numtasks;
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	if (argc != 6)
	{
		if (taskid == 0)
			std::cerr << "Invalid argument count. Usage: " << argv[0] << " <N> <xmin> <xmax> <ymin> <ymax>\n";
		MPI_Finalize();
		return 1;
	}

	// N: grid size
	int N = std::atoi(argv[1]);
	double N_squared = static_cast<double>(N) * static_cast<double>(N);

	// rectangle dimensions
	double xmin = std::atof(argv[2]);
	double xmax = std::atof(argv[3]);
	double ymin = std::atof(argv[4]);
	double ymax = std::atof(argv[5]);

	// step size (space between grid points)
	double step_x = (xmax - xmin) / static_cast<double>(N);
	double step_y = (ymax - ymin) / static_cast<double>(N);

	// we divide only the y coordinate in numtasks
	int slabs = N / numtasks;
	int remainder = N % numtasks;

	// start and endpoint for each process
	int my_start = taskid * slabs;
	int my_end = my_start + slabs;

	// if blocks could not be equally loaded, give the remainder to last task
	if (taskid == numtasks - 1)
	{
		my_end += remainder;
	}

	// sum is local, global_sum is what each process will communicate to
	double sum = 0.0;
	double global_sum = 0.0;

	double start_time = MPI_Wtime();

	// main computation of sum. may overflow if N is too big
	for (int j = my_start; j < my_end; ++j)
	{
		double curr_y = ymin + j * step_y;

		for (int i = 0; i < N; ++i)
		{
			double curr_x = xmin + i * step_x;
			sum += T(curr_x, curr_y);
		}
	}

	// sync sums to global sum
	MPI_Allreduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	double end_time = MPI_Wtime();
	double t = end_time - start_time;

	// master node prints results
	if (taskid == 0)
	{
		std::cout << "Execution time: " << t * 1000 << " ms\n";
		std::cout << "Average: " << global_sum / N_squared << "\n";
	}

	MPI_Finalize();
	return 0;
}