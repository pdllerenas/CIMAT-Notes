#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

class LCA
{
private:
  int n, l;
  vector<vector<int>> adj;

  int timer;
  vector<int> tin, tout, depth;
  vector<vector<int>> up;

  void dfs(int v, int p, int d)
  {
    tin[v] = ++timer;
    depth[v] = d;
    up[v][0] = p;
    for (int i = 1; i <= l; i++)
    {
      up[v][i] = up[up[v][i - 1]][i - 1];
    }

    for (int u : adj[v])
    {
      if (u != p)
      {
        dfs(u, v, d + 1);
      }
    }

    tout[v] = ++timer;
  }

  bool isAncestor(int u, int v)
  {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
  }

public:
  LCA(int _n) : n(_n)
  {
    adj.resize(n + 1);
    depth.resize(n + 1);
  }

  int lca(int u, int v)
  {
    if (isAncestor(u, v))
      return u;
    if (isAncestor(v, u))
      return v;
    for (int i = l; i >= 0; --i)
    {
      if (!isAncestor(up[u][i], v))
        u = up[u][i];
    }
    return up[u][0];
  }

  int getDistance(int u, int v)
  {
    int ancestor = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[ancestor];
  }

  void addEdge(int u, int v)
  {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void preprocess(int root)
  {
    tin.resize(n + 1);
    tout.resize(n + 1);
    timer = 0;
    l = ceil(log2(n));
    up.assign(n + 1, vector<int>(l + 1));
    dfs(root, root, 0);
  }
};

void solve()
{
  int n, q;
  cin >> n >> q;
  LCA t(n);
  for (int i = 0; i < n - 1; i++)
  {
    int a, b;
    cin >> a >> b;
    t.addEdge(a, b);
  }

  //
  t.preprocess(1);
  for (int i = 0; i < q; i++)
  {
    int u, v;
    cin >> u >> v;
    cout << t.getDistance(u, v) << '\n';
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}