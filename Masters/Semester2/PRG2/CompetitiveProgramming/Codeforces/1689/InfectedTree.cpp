#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

class Graph
{
private:
  int n;
  vector<vector<int>> adj;
  vector<int> sz; // store size of subtree
  vector<int> dp; // store max number of nodes saves

public:
  Graph(int _n) : n(_n)
  {
    adj.assign(n + 1, vector<int>());
    sz.assign(n + 1, 0);
    dp.assign(n + 1, 0);
  }

  void addEdge(int u, int v)
  {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void dfs(int u, int p)
  {
    sz[u] = 1; // at least itself
    vector<int> children;

    for (int v : adj[u])
    {
      if (v != p)
      {
        dfs(v, u);
        sz[u] += sz[v]; // add children size
        children.push_back(v);
      }
    }

    if (children.empty())
    {
      dp[u] = 0; // if no children, cant save any from subtree
    }
    else if (children.size() == 1)
    {
      int v = children[0];
      dp[u] = sz[v] - 1; // if only 1 child, can save child and whole subtree
    }
    else if (children.size() == 2)
    {
      int v1 = children[0], v2 = children[1];
      dp[u] = max(sz[v1] - 1 + dp[v2], dp[v1] + sz[v2] - 1); // if 2 children, find max when deleting one of them
    }
  }

  vector<int> get_dp() const
  {
    return dp;
  }
};

void solve()
{
  int n;
  cin >> n;
  Graph g(n);

  for (int i = 0; i < n - 1; i++)
  {
    int u, v;
    cin >> u >> v;
    g.addEdge(u, v);
  }

  g.dfs(1, 0);
  vector<int> dp = g.get_dp();
  cout << dp[1] << '\n'; // print dp of the root
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;

  while (t--)
  {
    solve();
  }

  return 0;
}