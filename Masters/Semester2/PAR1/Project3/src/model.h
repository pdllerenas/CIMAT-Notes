#include <vector>
#include <cmath>
#include <algorithm>
#include "types.h"

double V(double i, const Point &p)
{
  double Eo = p[0];
  double b = p[1];
  double Re = p[2];
  double C1 = p[3];
  double C2 = p[4];

  // prevent i from going below 0
  double safe_i = (i <= 0.0) ? 1e-9 : i;

  // prevent 1 - C2 * i from going below 0
  double ln_arg = 1.0 - C2 * safe_i;
  if (ln_arg <= 0.0)
  {
    ln_arg = 1e-9;
  }

  // E_cell = Eo - b*log(i) - Re*i + C1*ln(1 - C2*i)
  double V_est = Eo - b * std::log10(safe_i) - Re * safe_i + C1 * std::log(ln_arg);

  return V_est;
}

double Michalewicz(const Point &x)
{
  return -std::sin(x[0]) * std::pow(sin(x[0] * x[0] / M_PI), 20) - std::sin(x[1]) * std::pow(sin(2 * x[1] * x[1] / M_PI), 20);
}

double Rosenbrock(const Point &x)
{
  return std::pow(1.0 - x[0], 2) + 100.0 * std::pow(x[1] - std::pow(x[0], 2), 2);
}

// Sum of Squared Errors (SSE)
double objective_SSE(const Point &theta, const std::vector<DataPoint> &experiment_data)
{
  double sse = 0.0;

  for (int k = 0; k < experiment_data.size(); ++k)
  {
    double current_i = experiment_data[k].i;
    double real_voltage = experiment_data[k].VFC_i;

    double estimated_voltage = V(current_i, theta);

    sse += std::pow(real_voltage - estimated_voltage, 2);
  }

  return sse;
}

// Sum of Absolute Errors (SAE)
double objective_SAE(const Point &theta, const std::vector<DataPoint> &data)
{
  double sae = 0.0;
  for (const auto &point : data)
  {
    double V_est = V(point.i, Point{theta[0], theta[1], theta[2], theta[3], theta[4]});
    sae += std::abs(point.VFC_i - V_est);
  }
  return sae;
}

// Median Absolute Error (MAE)
double objective_MAE(const Point &theta, const std::vector<DataPoint> &data)
{
  std::vector<double> errors;
  errors.reserve(data.size());
  for (const auto &point : data)
  {
    double V_est = V(point.i, Point{theta[0], theta[1], theta[2], theta[3], theta[4]});
    errors.push_back(std::abs(point.VFC_i - V_est));
  }


  int n = errors.size();
  auto middle = errors.begin() + n / 2;
  std::nth_element(errors.begin(), middle, errors.end());

  if (n % 2 == 0)
  {
    auto max_left = std::max_element(errors.begin(), middle);
    return (*max_left + *middle) / 2.0;
  }
  else
  {
    return *middle;
  }
}