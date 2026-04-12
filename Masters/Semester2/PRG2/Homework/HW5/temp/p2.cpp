#include "dag.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m;
  cin >> n >> m;

  DAG g(n);

  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    // 1-based input to 0-based index
    g.addEdge(u - 1, v - 1);
  }

  vector<vector<int>> all = g.getAllTopologicalSorts();
  for (auto &v : all) {
    for (int i : v) {
      cout << i + 1 << ' ';
    }
    cout << '\n';
  }
  return 0;
}
