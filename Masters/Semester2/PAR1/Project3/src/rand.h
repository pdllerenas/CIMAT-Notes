#ifndef RAND_H
#define RAND_H

#include <random>

class Random
{
private:
  std::uniform_real_distribution<double> dis;
  std::mt19937 gen;

public:
  Random(int seed) : gen(seed), dis(0.0, 1.0) {}

  Random(double lower, double upper) : gen(std::random_device{}()), dis(lower, upper) {}

  Random() : gen(std::random_device{}()), dis(0.0, 1.0) {}

  double GetRand(double lower, double upper)
  {
    return lower + (upper - lower) * dis(gen);
  }

  double GetRand()
  {
    return dis(gen);
  }
};

#endif