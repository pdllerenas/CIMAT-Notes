#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;

  while (N--)
  {
    int n;
    cin >> n;
    if (n == 1)
    {
      cout << "0.000\n";
    }
    else if (n == 2)
    {
      cout << "4.000\n";
    }
    else
    {
      cout << fixed << setprecision(3) << (n % 2 == 0 ? n * n : n * n + 0.414) << "\n";
    }
  }

  return 0;
}