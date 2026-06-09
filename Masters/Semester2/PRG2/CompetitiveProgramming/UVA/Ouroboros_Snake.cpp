#include <iostream>
#include <vector>

using namespace std;

// cache for our De Bruijn sequences
vector<bool> seq[22];
int a[30];

// FKM Algorithm to generate Lyndon words
void FKM(int t, int p, int n)
{
  if (t > n)
  {
    // if length of Lyndon word divides n, append to sequence
    if (n % p == 0)
    {
      for (int i = 1; i <= p; i++)
      {
        seq[n].push_back(a[i]);
      }
    }
  }
  else
  {
    a[t] = a[t - p];
    FKM(t + 1, p, n);

    // increase character to find the next valid Lyndon word
    for (int j = a[t - p] + 1; j <= 1; j++)
    {
      a[t] = j;
      FKM(t + 1, t, n);
    }
  }
}

void precompute(int n)
{
  if (!seq[n].empty())
    return;

  seq[n].reserve(1 << n);
  a[0] = 0;
  FKM(1, 1, n);
}

void solve()
{
  int cases;
  if (!(cin >> cases))
    return;

  while (cases--)
  {
    int n, k;
    cin >> n >> k;

    precompute(n);

    int val = 0;
    int len = 1 << n;

    // get n-bit window starting at position k
    for (int i = 0; i < n; i++)
    {
      int bit = seq[n][(k + i) % len];
      val = (val << 1) | bit;
    }

    cout << val << "\n";
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}