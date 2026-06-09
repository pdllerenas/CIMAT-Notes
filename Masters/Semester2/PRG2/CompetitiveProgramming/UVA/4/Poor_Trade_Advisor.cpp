#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge
{
  int u, v, w;
};

// dfs that finds the component size
int dfs(int u, const vector<vector<int>> &adj, vector<bool> &visited)
{
  visited[u] = true;
  int count = 1;

  for (int v : adj[u])
  {
    if (!visited[v])
    {
      count += dfs(v, adj, visited);
    }
  }
  return count;
}

void solve()
{
  int n, m;
  while (cin >> n >> m && (n != 0 || m != 0))
  {
    vector<Edge> edges(m);
    int global_max = INT_MIN;
    for (int i = 0; i < m; i++)
    {
      cin >> edges[i].u >> edges[i].v >> edges[i].w;
      if (edges[i].w > global_max)
      {
        global_max = edges[i].w;
      }
    }

    // only add edges which will not reduce the average
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++)
    {
      if (edges[i].w == global_max)
      {
        adj[edges[i].u].push_back(edges[i].v);
        adj[edges[i].v].push_back(edges[i].u);
      }
    }

    vector<bool> visited(n + 1, false);
    int max_cities = 0;

    // find biggest component in graph
    for (int i = 1; i < n + 1; i++)
    {
      if (!visited[i])
      {
        if (!adj[i].empty())
        {
          int current_component_size = dfs(i, adj, visited);
          max_cities = max(max_cities, current_component_size);
        }
      }
    }
    cout << max_cities << '\n';
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}