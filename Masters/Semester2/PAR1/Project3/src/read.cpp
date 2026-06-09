#include <iostream>
#include <fstream>

#include "read.h"

std::vector<DataPoint> read_experimental_data(const std::string &filename)
{
  std::vector<DataPoint> data;
  std::ifstream file(filename);
  double current_density, voltage;

  if (file.is_open())
  {
    // Reads line by line, assuming space or tab-separated values
    while (file >> current_density >> voltage)
    {
      data.push_back({current_density, voltage});
    }
    file.close();
  }
  else
  {
    std::cerr << "Error: Unable to open " << filename << std::endl;
  }
  return data;
}

HParams read_parameters_data(const std::string &filename)
{
  std::ifstream file(filename);

  int ndim;
  int HS_size;
  double HMCR;
  double PAR;
  int MaxAttemps;

  std::vector<double> lower_bound;
  std::vector<double> upper_bound;
  std::vector<double> pa_range;

  if (file.is_open())
  {
    // Reads line by line, assuming space or tab-separated values
    file >> ndim;
    file >> HS_size;
    file >> HMCR;
    file >> PAR;
    file >> MaxAttemps;

    lower_bound.resize(ndim);
    upper_bound.resize(ndim);
    pa_range.resize(ndim);

    for (int i = 0; i < ndim; i++)
    {
      file >> lower_bound[i];
    }

    for (int i = 0; i < ndim; i++)
    {
      file >> upper_bound[i];
    }

    for (int i = 0; i < ndim; i++)
    {
      file >> pa_range[i];
    }

    file.close();
  }
  else
  {
    std::cerr << "Error: Unable to open " << filename << std::endl;
  }

  HParams data{ndim, HS_size, HMCR, PAR, MaxAttemps, lower_bound, upper_bound, pa_range};
  return data;
}