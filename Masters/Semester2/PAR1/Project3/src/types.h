#ifndef TYPES_H
#define TYPES_H
#include <vector>
#include <functional>
#include <cstdint>

// x in RR^n
using Point = std::vector<double>;

// f : RR^n -> RR
using Func = std::function<double(const Point &)>;
using FuncHP = std::function<double(double, const Point &)>;


struct DataPoint {
    double i;       // Current density
    double VFC_i;   // Observed voltage
};

typedef struct HParams
{
  int ndim;
  int HS_size;
  double HMacceptRate;
  double PArate;
  int MaxAttempt;

  std::vector<double> lower_bound;
  std::vector<double> upper_bound;
  std::vector<double> pa_range;

  // default values for Rosenbrock function testing
  HParams() : ndim(2), HS_size(20), HMacceptRate(0.95), PArate(0.7), MaxAttempt(25000)
  {
    lower_bound = {-5.0, -5.0};
    upper_bound = {5.0, 5.0};
    pa_range = {100.0, 100.0};
  }

  HParams(int _ndim,
          int _HS_size,
          double _HMacceptRate,
          double _PArate,
          int _MaxAttempt,
          std::vector<double> _lower_bound,
          std::vector<double> _upper_bound,
          std::vector<double> _pa_range) : ndim(_ndim),
                                           HS_size(_HS_size),
                                           HMacceptRate(_HMacceptRate),
                                           PArate(_PArate),
                                           MaxAttempt(_MaxAttempt),
                                           lower_bound(_lower_bound),
                                           upper_bound(_upper_bound),
                                           pa_range(_pa_range) {}
} HParams;
#endif