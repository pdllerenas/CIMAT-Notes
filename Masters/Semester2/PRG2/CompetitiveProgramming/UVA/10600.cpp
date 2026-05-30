#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class DSU
{
  vector<int> parent, rank;

public:
  DSU(int n)
  {
    parent.resize(n);
    rank.resize(n, 1);
    for (int i = 0; i < n; i++)
    {
      parent[i] = i;
    }
  }

  int find(int i)
  {
    return (parent[i] == i ? i : (parent[i] = find(parent[i])));
  }

  bool unite(int x, int y)
  {
    int s1 = find(x), s2 = find(y);
    if (s1 != s2)
    {
      if (rank[s1] < rank[s2])
      {
        parent[s1] = s2;
      }
      else if (rank[s1] > rank[s2])
      {
        parent[s2] = s1;
      }
      else
      {
        parent[s2] = s1, rank[s1]++;
      }
      return true;
    }
    return false;
  }
};

bool comparator(vector<int> &a, vector<int> &b)
{
  return a[2] < b[2];
}

void kruskalMST(int V, vector<vector<int>> &edges)
{
  int M = edges.size();
  sort(edges.begin(), edges.end(), comparator);
  DSU dsu(V + 1);
  int cost1 = 0, count = 0;
  vector<int> mst_edges;

  for (int i = 0; i < M; i++)
  {
    int x = edges[i][0], y = edges[i][1], w = edges[i][2];
    if (dsu.unite(x, y))
    {
      cost1 += w;
      mst_edges.push_back(i);
      if (++count == V - 1)
        break;
    }
  }

  int cost2 = 1e9;

  for (int skipped : mst_edges)
  {
    DSU dsu2(V + 1);
    int curr_cost = 0;
    int curr_edges = 0;

    for (int i = 0; i < M; i++)
    {
      if (i == skipped)
      {
        continue;
      }
      if (dsu2.unite(edges[i][0], edges[i][1]))
      {
        curr_cost += edges[i][2];
        curr_edges++;
        if (curr_edges == V - 1)
          break;
      }
    }
    if (curr_edges == V - 1)
      cost2 = min(cost2, curr_cost);
  }

  cout << cost1 << ' ' << cost2 << endl;
}

int main()
{
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  while (t--)
  {
    int N, M;
    cin >> N >> M;
    vector<vector<int>> edges(M);
    for (int i = 0; i < M; i++)
    {
      int A, B, C;
      cin >> A >> B >> C;
      edges[i] = {A, B, C};
    }
    kruskalMST(N, edges);
  }

  return 0;
}