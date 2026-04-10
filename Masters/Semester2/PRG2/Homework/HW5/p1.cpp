#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

void findSCC(int u, int &timer, vector<int> &disc, vector<int> &low,
             stack<int> &st, vector<bool> &onStack,
             const vector<vector<int>> &adj, vector<int> &sccID,
             int &sccCount) {
  disc[u] = low[u] = ++timer;
  st.push(u);
  onStack[u] = true;

  for (int v : adj[u]) {
    if (disc[v] == 0) {
      findSCC(v, timer, disc, low, st, onStack, adj, sccID, sccCount);
      low[u] = min(low[u], low[v]);
    } else if (onStack[v]) {
      low[u] = min(low[u], disc[v]);
    }
  }

  if (low[u] == disc[u]) {
    while (true) {
      int v = st.top();
      st.pop();
      onStack[v] = false;
      sccID[v] = sccCount;
      if (u == v)
        break;
    }
    sccCount++;
  }
}

int solve() {
  int n, m;
  if (!(cin >> n >> m))
    return 0;
  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
  }

  vector<int> disc(n + 1, 0), low(n + 1, 0), sccID(n + 1, 0);
  vector<bool> onStack(n + 1, false);
  stack<int> st;
  int timer = 0, sccCount = 0;

  for (int i = 1; i <= n; ++i)
    if (disc[i] == 0)
      findSCC(i, timer, disc, low, st, onStack, adj, sccID, sccCount);

  // check in-degree of each SCC
  vector<int> inDegreeSCC(sccCount, 0);
  for (int u = 1; u <= n; ++u) {
    for (int v : adj[u]) {
      if (sccID[u] != sccID[v]) {
        inDegreeSCC[sccID[v]]++;
      }
    }
  }

  // count SCCs with in-degree 0
  int min_algorithms = 0;
  for (int i = 0; i < sccCount; ++i) {
    if (inDegreeSCC[i] == 0)
      min_algorithms++;
  }

  return min_algorithms;
}

int main() { std::cout << solve() << std::endl; }
