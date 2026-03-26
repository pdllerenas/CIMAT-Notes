#include <iostream>
#include <vector>
#include <unordered_map>
#include "DAG.h"

using matrix = std::vector<std::vector<int>>;

bool bip_match(int u, const matrix &adj, std::vector<bool> &seen, std::vector<int> &matchR)
{
  for (int v : adj[u])
  {
    if (!seen[v])
    {
      seen[v] = true;
      // if not matched or can restructure matched nodes
      if (matchR[v] < 0 || bip_match(matchR[v], adj, seen, matchR))
      {
        matchR[v] = u;
        return true;
      }
    }
  }
  return false;
}

int minPathCover(const matrix &adj, int V)
{
  // -1 represents no incoming edge
  std::vector<int> matchR(V, -1);
  int max_matching = 0;

  for (int i = 0; i < V; i++)
  {
    std::vector<bool> seen(V, false);

    if (bip_match(i, adj, seen, matchR))
    {
      max_matching++;
    }
  }

  std::unordered_map<int, int> matchL;
  std::vector<bool> hasIncomingMatch(V, false);

  for (int v = 0; v < V; v++)
  {
    if (matchR[v] != -1)
    {
      int u = matchR[v];
      matchL[u] = v;
      hasIncomingMatch[v] = true;
    }
  }


  std::cout << "Min Path Count:\n";
  std::cout << V - max_matching << '\n';

  std::cout << "PATHS:\n";
  std::vector<bool> used(V, false);

  for (int i = 0; i < V; i++)
  {
    if (!hasIncomingMatch[i])
    {
      int current = i;
      std::cout << current;
      used[current] = true;

      while (matchL.count(current))
      {
        current = matchL[current];
        std::cout << " -> " << current;
        used[current] = true;
      }
      std::cout << '\n';
    }
  }
  std::cout << '\n';

  return V - max_matching;
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  DAG g(N);

  int E;
  std::cin >> E;

  int a,b;
  while (E--) {
    std::cin >> a >> b;
    if (a == b) {
      std::cerr << "Cannot add self loop.\n";
      N++;
      continue;
    }
    g.addEdge(a, b);
  }

  if (g.isDAG())
  {
    int mpc = minPathCover(g.getAdjacencyMatrix(), g.getVertices());
  }
  else
  {
    std::cout << "The graph has a cycle." << std::endl;
  }

  return 0;
}