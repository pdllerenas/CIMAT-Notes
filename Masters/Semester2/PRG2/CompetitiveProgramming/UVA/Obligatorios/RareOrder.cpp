#include <string>
#include <vector>
#include <iostream>
#include <stack>

using namespace std;

class DAG
{
private:
  // adjacency matrix for each char of alphabet (english letter only, so 26)
  vector<int> adj[26];

  // keeps track of letters used in the given list of words
  bool exists[26] = {false};

  // Stack contains the sorted list
  void topologicalSort(int v, vector<bool> &visited, stack<int> &Stack)
  {
    visited[v] = true;
    for (int neighbor : adj[v])
    {
      if (!visited[neighbor])
      {
        topologicalSort(neighbor, visited, Stack);
      }
    }
    Stack.push(v);
  }

public:
  DAG()
  {
  }

  void addEdge(int u, int v)
  {
    adj[u].push_back(v);
  }

  // marks the character as existent in our dictionary
  void mark(int u)
  {
    exists[u] = true;
  }

  void topologicalSort()
  {
    stack<int> Stack;
    vector<bool> visited(26, false);
    for (int i = 0; i < 26; i++)
    {
      // only process the letter if it is in our dictionary
      if (exists[i] && !visited[i])
      {
        topologicalSort(i, visited, Stack);
      }
    }
    // print topological sort
    while (!Stack.empty())
    {
      cout << (char)(Stack.top() + 'A');
      Stack.pop();
    }
    cout << "\n";
  }
};

void solve()
{
  string s;
  string prev = "";

  DAG g;
  while (cin >> s && s != "#")
  {
    // mark used letters
    for (char c : s)
    {
      g.mark(c - 'A');
    }

    if (prev.empty())
    {
      prev = s;
      continue;
    }
    // compare char by char and determine the topology of the alphabet
    for (int i = 0; i < min(s.size(), prev.size()); i++)
    {
      if (s[i] != prev[i])
      {
        g.addEdge(prev[i] - 'A', s[i] - 'A');
        break;
      }
    }
    prev = s;
  }
  g.topologicalSort();
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();
  return 0;
}