#include <iostream>
#include <vector>

using namespace std;

void solve()
{
  int n;

  while (cin >> n && n != 0)
  {
    vector<vector<long long>> a(n, vector<long long>(n));
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        cin >> a[i][j];
      }
    }

    long long det = 1;

    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        while (a[j][i] != 0)
        {
          long long q = a[i][i] / a[j][i];

          // row i = row i - q * row j
          for (int k = i; k < n; k++)
          {
            a[i][k] -= q * a[j][k];
          }

          for (int k = i; k < n; k++)
          {
            swap(a[i][k], a[j][k]);
          }

          det = -det;
        }
      }

      // if the pivot is 0 after clearing the column, the matrix is singular
      if (a[i][i] == 0)
      {
        det = 0;
        break;
      }

      det *= a[i][i];
    }

    cout << det << '\n';
  }

  cout << "*\n";
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}