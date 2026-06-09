#include <iostream>
#include <cmath>
using namespace std;

void solve()
{
  int n, m;
  cin >> n >> m;
  if (n == m)
  {
    int k = (n + 1) / 2;
    cout << k * (k + 1) / 2 << '\n';
  }
  else
  {
    int r = (n + 1) / 2;
    int c = (m + 1) / 2;
    cout << r * c << '\n';
  }
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