#pragma once

#include <iostream>
#include <vector>
#include <stack>

class DAG
{
private:
  int _V;
  std::vector<std::vector<int>> _adj;

  bool _isCyclic(int v, std::vector<bool> &visited, std::vector<bool> &recStack) const
  {
    if (!visited[v])
    {
      visited[v] = true;
      recStack[v] = true;

      for (int neighbor : _adj[v])
      {
        if (!visited[neighbor] && _isCyclic(neighbor, visited, recStack))
        {
          return true;
        }
        else if (recStack[neighbor])
        {
          return true;
        }
      }
    }
    recStack[v] = false;
    return false;
  }

  void _topologicalSort(int v, std::vector<bool> &visited, std::stack<int> &Stack) const
  {
    visited[v] = true;

    for (int neighbor : _adj[v])
    {
      if (!visited[neighbor])
      {
        _topologicalSort(neighbor, visited, Stack);
        visited[neighbor] = true;
      }
    }
    Stack.push(v);
  }

public:
  DAG(int vertices)
  {
    _V = vertices;
    _adj.resize(_V);
  }

  void addEdge(int u, int v)
  {
    _adj[u].push_back(v);
  }

  bool isDAG() const
  {
    std::vector<bool> visited(_V, false);
    std::vector<bool> recStack(_V, false);

    for (int i = 0; i < _V; i++)
    {
      if (!visited[i] && _isCyclic(i, visited, recStack))
      {
        return false;
      }
    }
    return true;
  }

  void topologicalSort() const
  {
    if (!isDAG())
    {
      return;
    }
    std::stack<int> Stack;
    std::vector<bool> visited(_V, false);

    for (int i = 0; i < _V; i++)
    {
      if (!visited[i])
      {
        _topologicalSort(i, visited, Stack);
      }
    }

    std::cout << "Topological sort:\n";
    while (!Stack.empty())
    {
      std::cout << Stack.top() << " ";
      Stack.pop();
    }
    std::cout << "\n";
  }

  std::vector<std::vector<int>> getAdjacencyMatrix() const {
    return _adj;
  }

  int getVertices() const {
    return _V;
  }
};
