#include <cstdint>
#include <iostream>
#include <stack>
#include <vector>

class DAG {
private:
  std::size_t _V;
  std::vector<std::vector<int>> _adj;
  bool isCyclic(int v, std::vector<bool> &visited,
                std::vector<bool> &recStack) {
    if (!visited[v]) {
      visited[v] = true;
      recStack[v] = true;
    }

    for (int neighbor : _adj[v]) {
      if (!visited[neighbor] && isCyclic(neighbor, visited, recStack)) {
        return true;
      } else if (recStack[neighbor]) {
        return true;
      }
    }
    recStack[v] = false;
    return false;
  }

  void topologicalSort(int v, std::vector<bool> &visited,
                       std::stack<int> &Stack) {
    visited[v] = true;

    for (int neighbor : _adj[v]) {
      if (!visited[neighbor]) {
        topologicalSort(neighbor, visited, Stack);
      }
    }

    Stack.push(v);
  }

  void allTopologicalSortsUtil(std::vector<int> &path,
                               std::vector<bool> &visited,
                               std::vector<int> &in_degree,
                               std::vector<std::vector<int>> &all_sorts) {
    bool found = false;
    for (int i = 0; i < _V; i++) {
      // we pick the node if no ancestors and not visited
      if (in_degree[i] == 0 && !visited[i]) {
        visited[i] = true;
        path.push_back(i);
        // once in path, simulate a deleting of the node (by reducing the in
        // degree of neighbors)
        for (int neighbor : _adj[i]) {
          in_degree[neighbor]--;
        }

        // recursive exploration
        allTopologicalSortsUtil(path, visited, in_degree, all_sorts);

        // re-insert to neighbors (backtracking)
        for (int neighbor : _adj[i]) {
          in_degree[neighbor]++;
        }
        path.pop_back();
        visited[i] = false;

        found = true;
      }
    }

    // base case
    if (!found && path.size() == _V) {
      all_sorts.push_back(path);
    }
  }

public:
  DAG(int V) : _V(V) { _adj.resize(_V); }
  void addEdge(int u, int v) { _adj[u].push_back(v); }
  bool isDAG() {
    std::vector<bool> visited(_V, false);
    std::vector<bool> recStack(_V, false);

    for (int i = 0; i < _V; i++) {
      if (!visited[i] && isCyclic(i, visited, recStack)) {
        return false;
      }
    }
    return true;
  }

  int topologicalSort() {
    if (!isDAG()) {
      std::cerr << "Cycle found\n";
      return -1;
    }

    std::stack<int> Stack;
    std::vector<bool> visited(_V, false);

    int components = 0;
    for (int i = 0; i < _V; i++) {
      if (!visited[i]) {
        topologicalSort(i, visited, Stack);
        components++;
      }
    }

    return components;
  }

  std::vector<std::vector<int>> getAllTopologicalSorts() {
    std::vector<int> in_degree(_V, 0);

    for (int i = 0; i < _V; i++) {
      for (int neighbor : _adj[i]) {
        in_degree[neighbor]++;
      }
    }
		std::vector<bool> visited(_V, false);
		std::vector<int> path;
		std::vector<std::vector<int>> all_sorts;

    allTopologicalSortsUtil(path, visited, in_degree, all_sorts);
		
		return all_sorts;
  }

  std::vector<std::vector<int>> getAdjList() { return _adj; }
};
