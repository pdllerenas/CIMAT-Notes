#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 1000000;
int divs[MAXN + 5];
int ans[MAXN + 5];

void precompute()
{
  // sieve to count divisors for all numbers up to MAXN
  for (int i = 1; i <= MAXN; i++)
  {
    for (int j = i; j <= MAXN; j += i)
    {
      divs[j]++;
    }
  }

  // build the prefix array to answer queries in O(1)
  int max_divs = 0;
  int best_num = 0;

  for (int i = 1; i <= MAXN; i++)
  {
    if (divs[i] >= max_divs)
    {
      max_divs = divs[i];
      best_num = i;
    }
    ans[i] = best_num;
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  precompute();

  int t;
  if (cin >> t)
  {
    while (t--)
    {
      int n;
      cin >> n;
      cout << ans[n] << '\n';
    }
  }

  return 0;
}