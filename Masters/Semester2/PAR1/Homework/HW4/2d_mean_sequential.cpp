#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>

double T(double x, double y)
{
	return 20.0 * (std::sin(4.0 * x) * std::sin(3.0 * y) + 0.3 * std::cos(6.0 * x) * std::sin(5.0 * y) + 1);
}

int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		std::cerr << "Invalid argument count. Usage: " << argv[0] << " <N> <xmin> <xmax> <ymin> <ymax>\n";
		return 1;
	}

	int N = atoi(argv[1]);

	double xmin = atoi(argv[2]);
	double xmax = atoi(argv[3]);
	double ymin = atoi(argv[4]);
	double ymax = atoi(argv[5]);

	double curr_x, curr_y;
	double step_x = (xmax - xmin) / static_cast<double>(N);
	double step_y = (ymax - ymin) / static_cast<double>(N);

	double sum = 0.0;

	auto ti = std::chrono::steady_clock::now();
	for (curr_y = ymin; curr_y <= ymax; curr_y += step_y)
	{
		for (curr_x = xmin; curr_x <= xmax; curr_x += step_x)
		{
			sum += T(curr_x, curr_y);
		}
	}
	auto tf = std::chrono::steady_clock::now();

	std::chrono::duration<double> t = tf - ti;
	double time = t.count();

	std::cout << "Execution time: " << time * 1000 << " ms" << std::endl;
	std::cout << "Average: " << sum / static_cast<double>(N * N) << std::endl;
}