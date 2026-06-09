#include <iostream>
#include <stack>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

void dfs(int N, int last_d, stack<int> &factors, vector<vector<int>> &results)
{
  // check only divisors geq to last used divisor, up to sqrt(N), equivalent to d*d <= N
  for (int d = last_d; d * d <= N; d++)
  {
    if (N % d == 0)
    {
      factors.push(d);
      dfs(N / d, d, factors, results);
      factors.pop();
    }
  }

  // if we have checked all divisors and factors still has elements (e.g., 12 = 2 * 2 * 3),
  // when we are at 3, we cant divide it, but stack = [2,2], and so this is the last element available in the
  // recursion tree
  if (N > 1 && !factors.empty())
  {
    factors.push(N);

    // recover path
    stack<int> temp = factors;
    vector<int> path;

    while (!temp.empty())
    {
      path.push_back(temp.top());
      temp.pop();
    }

    // reverse so we start at smallest factor, since stack popped biggest elements first
    reverse(path.begin(), path.end());
    results.push_back(path);

    // once we are done with this leaf, check other paths
    factors.pop();
  }
}

void solve()
{
  int N;
  while (cin >> N && N != 0)
  {
    stack<int> factors;
    vector<vector<int>> results;

    dfs(N, 2, factors, results);
    cout << results.size() << '\n';

    for (const auto &r : results)
    {
      for (size_t i = 0; i < r.size(); i++)
      {
        cout << r[i] << (i + 1 == r.size() ? "" : " ");
      }
      cout << '\n';
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