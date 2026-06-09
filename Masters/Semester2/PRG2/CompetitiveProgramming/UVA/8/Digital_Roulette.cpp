#include <vector>
#include <iostream>
using namespace std;

int seen[10000000 + 5];

void solve()
{
  long long N, M;
  int t = 1;
  while (cin >> N >> M && (N != 0 || M != 0))
  {
    int k;
    cin >> k;

    vector<long long> a(k + 1);
    for (int i = 0; i <= k; i++)
    {
      cin >> a[i];
    }

    long long mod = N + 1;
    int unique_results = 0;

    // find polynomial value for all force values
    for (long long x = 0; x <= M; x++)
    {
      long long res = 0;

      // horner method
      for (int i = k; i >= 0; i--)
      {
        res = (res * x + a[i]) % mod;
      }

      // reset state without memset
      if (seen[res] != t)
      {
        seen[res] = t;
        unique_results++;
      }
    }

    cout << unique_results << '\n';
    t++;
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}