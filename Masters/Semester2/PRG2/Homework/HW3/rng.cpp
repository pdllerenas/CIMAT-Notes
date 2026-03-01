#include <iostream>
#include <random>

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  unsigned int n;
  std::cin >> n;

  uint64_t seed;
  std::cin >> seed;

  double x_min, x_max, y_min, y_max;
  std::cin >> x_min >> x_max >> y_min >> y_max;

  std::mt19937 mt{seed};

  for (int count{1}; count <= 40; ++count)
  {
    std::cout << mt() << '\t';

    if (count % 5 == 0)
      std::cout << '\n';
  }
  return 0;
}