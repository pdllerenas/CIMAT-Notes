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

	int N = std::atoi(argv[1]);
	double N_squared = static_cast<double>(N) * static_cast<double>(N);

	double xmin = std::atof(argv[2]);
	double xmax = std::atof(argv[3]);
	double ymin = std::atof(argv[4]);
	double ymax = std::atof(argv[5]);

	// step size (space between grid points)
	double step_x = (xmax - xmin) / static_cast<double>(N);
	double step_y = (ymax - ymin) / static_cast<double>(N);

	int slabs = N / numtasks;
	int remainder = N % numtasks;

	int start = taskid * slabs;
	int end = start + slabs;

	// if blocks could not be equally loaded, give the remainder to last task
	if (taskid == numtasks - 1)
	{
		end += remainder;
	}

	// local variable for each process
	double sum = 0.0;

	// global variable the parent process maintains
	double global_sum = 0.0;

	double start_time = MPI_Wtime();

	for (int j = start; j < end; ++j)
	{
		double curr_y = ymin + j * step_y;

		for (int i = 0; i < N; ++i)
		{
			double curr_x = xmin + i * step_x;
			sum += T(curr_x, curr_y);
		}
	}

	// sync sums to global sum
	MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	double end_time = MPI_Wtime();
	double t = end_time - start_time;

	if (taskid == 0)
	{
		std::cout << "Execution time: " << t * 1000 << " ms\n";
		std::cout << "Average: " << global_sum / N_squared << "\n";
	}

	MPI_Finalize();
	return 0;
}