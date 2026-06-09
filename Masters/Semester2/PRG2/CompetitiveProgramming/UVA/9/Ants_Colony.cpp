#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 100005;
const int LOG = 18;

vector<pair<int, long long>> adj[MAXN];
int up[MAXN][LOG];
int depth[MAXN];
long long dist[MAXN];

void dfs(int u, int p, int d, long long current_dist)
{
  up[u][0] = p;
  depth[u] = d;
  dist[u] = current_dist;

  for (int i = 1; i < LOG; i++)
  {
    up[u][i] = up[up[u][i - 1]][i - 1];
  }

  for (auto edge : adj[u])
  {
    int v = edge.first;
    long long w = edge.second;
    if (v != p)
    {
      dfs(v, u, d + 1, current_dist + w);
    }
  }
}

int get_lca(int u, int v)
{
  if (depth[u] < depth[v])
  {
    swap(u, v);
  }

  // lift u up to the exact same depth as v
  int diff = depth[u] - depth[v];
  for (int i = 0; i < LOG; i++)
  {
    if ((diff >> i) & 1)
    {
      u = up[u][i];
    }
  }

  // if u and v were on the same branch, u is now the LCA
  if (u == v)
    return u;

  // lift both nodes simultaneously to just below the LCA
  for (int i = LOG - 1; i >= 0; i--)
  {
    if (up[u][i] != up[v][i])
    {
      u = up[u][i];
      v = up[v][i];
    }
  }

  // the parent of this level is the exact LCA
  return up[u][0];
}

void solve()
{
  int n;
  while (cin >> n && n != 0)
  {
    for (int i = 0; i < n; i++)
    {
      adj[i].clear();
    }

    for (int i = 1; i < n; i++)
    {
      int a;
      long long l;
      cin >> a >> l;
      adj[i].push_back({a, l});
      adj[a].push_back({i, l});
    }

    dfs(0, 0, 0, 0);

    int q;
    cin >> q;
    for (int i = 0; i < q; i++)
    {
      int u, v;
      cin >> u >> v;

      int lca = get_lca(u, v);
      long long ans = dist[u] + dist[v] - 2LL * dist[lca];

      cout << ans;
      if (i == q - 1)
        cout << "\n";
      else
        cout << " ";
    }
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}