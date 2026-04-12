#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

class TarjanSolver {
private:
  int numNodes;
  vector<vector<int>> adj;

  vector<int> discoveryTime;
  vector<int> lowestReachable;
  vector<int> sccID;
  vector<bool> onStack;
  stack<int> currentSCCStack;

  int currentTime;
  int sccCount;

  void findSCC(int u) {
    discoveryTime[u] = lowestReachable[u] = ++currentTime;
    currentSCCStack.push(u);
    onStack[u] = true;

    for (int v : adj[u]) {
      if (discoveryTime[v] == 0) {
        // if v hasn't been visited yet
        findSCC(v);
        lowestReachable[u] = min(lowestReachable[u], lowestReachable[v]);
      } else if (onStack[v]) {
        // if v is part of the current SCC
        lowestReachable[u] = min(lowestReachable[u], discoveryTime[v]);
      }
    }

    // if u is a root node of an SCC, pop the stack to extract the whole
    // component
    if (lowestReachable[u] == discoveryTime[u]) {
      while (true) {
        int v = currentSCCStack.top();
        currentSCCStack.pop();
        onStack[v] = false;
        sccID[v] = sccCount;

        if (u == v)
          break;
      }
      sccCount++;
    }
  }

public:
  TarjanSolver(int n)
      : numNodes(n), adj(n + 1), discoveryTime(n + 1, 0),
        lowestReachable(n + 1, 0), sccID(n + 1, 0), onStack(n + 1, false),
        currentTime(0), sccCount(0) {}

  void addEdge(int u, int v) { adj[u].push_back(v); }

  int getMinimumStartingNodes() {
    // find all SCCs
    for (int i = 1; i <= numNodes; ++i) {
      if (discoveryTime[i] == 0) {
        findSCC(i);
      }
    }

    // calculate the in-degree of each SCC in the condensed graph
    vector<int> inDegreeSCC(sccCount, 0);
    for (int u = 1; u <= numNodes; ++u) {
      for (int v : adj[u]) {
        // if the edge connects two entirely different SCCs
        if (sccID[u] != sccID[v]) {
          inDegreeSCC[sccID[v]]++;
        }
      }
    }

    // count how many SCCs have an in-degree of 0
    int minAlgorithms = 0;
    for (int i = 0; i < sccCount; ++i) {
      if (inDegreeSCC[i] == 0) {
        minAlgorithms++;
      }
    }

    return minAlgorithms;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m;
  TarjanSolver g(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    g.addEdge(u, v);
  }
  cout << g.getMinimumStartingNodes() << "\n";
  return 0;
}
