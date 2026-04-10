#include "dag.h"
#include <iostream>
#include <vector>

using std::vector;

int main() {
  unsigned long n, m;
  std::cin >> n >> m;

  DAG g(n);

  for (int i = 0; i < m; i++) {
    unsigned int p, q;
    std::cin >> p >> q;
    // 1-based input to 0-based index
    g.addEdge(p - 1, q - 1);
  }

  vector<vector<int>> all = g.getAllTopologicalSorts();
  for (auto &v : all) {
    for (int i : v) {
      std::cout << i + 1 << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}
