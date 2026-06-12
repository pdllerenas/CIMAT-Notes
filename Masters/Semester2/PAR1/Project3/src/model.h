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

double objective_Rosenbrock(const Point &x)
{
  double sum = 0.0;
  for (size_t i = 0; i < x.size() - 1; ++i)
  {
    double term1 = x[i + 1] - x[i] * x[i];
    double term2 = 1.0 - x[i];
    sum += 100.0 * term1 * term1 + term2 * term2;
  }
  return sum;
}

double objective_Michalewicz(const Point &x)
{
  double sum = 0.0;
  int m = 10;
  for (size_t i = 0; i < x.size(); ++i)
  {
    double term1 = sin(x[i]);
    double term2 = sin(((i + 1) * x[i] * x[i]) / M_PI);
    sum += term1 * pow(term2, 2 * m);
  }
  return -sum;
}

// Sum of Squared Errors (SSE)
double objective_SSE(const Point &theta, const std::vector<DataPoint> &experiment_data)
{
  double sse = 0.0;

#pragma omp parallel for reduction(+ : sse) default(none) shared(experiment_data, theta)
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
#pragma omp parallel for reduction(+ : sae) default(none) shared(theta, data)
  for (const auto &point : data)
  {
    double V_est = V(point.i, theta);
    sae += std::abs(point.VFC_i - V_est);
  }
  return sae;
}

// Median Absolute Error (MAE)
double objective_MAE(const Point &theta, const std::vector<DataPoint> &data)
{
  std::vector<double> errors(data.size());

#pragma omp parallel for default(none) shared(theta, data, errors)
  for (int k = 0; k < data.size(); k++)
  {
    double V_est = V(data[k].i, theta);
    errors[k] = std::abs(data[k].VFC_i - V_est);
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