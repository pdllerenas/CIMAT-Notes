#include <iostream>
#include <random>
#include "quadtree.h"

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  unsigned int n;
  std::cin >> n;

  if (n <= 0)
  {
    fprintf(stderr, "Invalid node count.\n");
    return 1;
  }

  uint64_t seed;
  std::cin >> seed;
  if (seed <= 0)
  {
    fprintf(stderr, "Invalid seed.\n");
    return 1;
  }

  double x_min, x_max, y_min, y_max;
  std::cin >> x_min >> x_max >> y_min >> y_max;

  if (x_min > x_max)
    std::swap(x_min, x_max);
  if (y_min > y_max)
    std::swap(y_min, y_max);

  std::mt19937 mt(seed);
  std::uniform_real_distribution<double> dis_x(x_min, x_max);
  std::uniform_real_distribution<double> dis_y(y_min, y_max);

  Rectangle rect = {x_min, x_max, y_min, y_max};

  std::vector<Point> points;
  points.reserve(n);

  for (int i = 0; i < n; i++)
  {
    points.push_back({dis_x(mt), dis_y(mt)});
  }

  std::size_t low = 1, high = n;
  std::size_t best_capacity = 1;
  while (low <= high)
  {
    std::size_t mid = (high - low) / 2 + low;

    QuadTree qt(rect, 0, mid);
    for (const auto &p : points)
      qt.insert(p);
    int sub = qt.count_subdivisions();
    if (sub >= 4)
    {
      best_capacity = mid;
      low = mid + 1;
    }
    else
    {
      high = mid - 1;
    }
  }

  std::cout << best_capacity << std::endl;
  // Generate and print the optimal tree
  // QuadTree optimal_qt(rect, 0, best_capacity);
  // for (const auto &p : points)
  //   optimal_qt.insert(p);

  // optimal_qt.print();

  // std::cout << "C,S(C),D(C)\n";
  // for (int C = 1; C < n; C++)
  // {
  //   QuadTree qt(rect, 0, C);
  //   for (const auto &p : points)
  //     qt.insert(p);

  //   std::cout << C << ',' << qt.count_subdivisions() << ',' << qt.get_max_depth() << '\n';
  // }

  return 0;
}