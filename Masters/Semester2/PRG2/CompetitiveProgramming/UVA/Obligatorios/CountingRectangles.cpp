#include <vector>
#include <iostream>

using namespace std;

void solve()
{
  int n, q;
  cin >> n >> q;

  // max value imposed on h and w
  const int MAX = 1005;

  // grid of values, where grid[h][w] accumulates the area of rectangles with
  // height h and width w. If there are 2 rectangles with the same dimensions,
  // then grid[h][w] = 2 * h * w
  vector<vector<long long>> grid(MAX, vector<long long>(MAX, 0));

  // 2d prefix sum with the usual formula:
  // prefix[i][j] = grid[i][j] + prefix[i-1][j] + prefix[i][j-1] - prefix[i-1][j-1]
  vector<vector<long long>> prefix(MAX, vector<long long>(MAX, 0));

  for (int i = 0; i < n; i++)
  {
    long long h, w;
    cin >> h >> w;
    grid[h][w] += h * w;
  }
  // omit [i][0] and [0][j] since these areas are 0 (preset when initializing)
  for (int i = 1; i < MAX; i++)
  {
    for (int j = 1; j < MAX; j++)
    {
      prefix[i][j] = grid[i][j] + prefix[i][j - 1] + prefix[i - 1][j] - prefix[i - 1][j - 1];
    }
  }

  for (int i = 0; i < q; i++)
  {
    long hs, hb, ws, wb;
    cin >> hs >> ws;
    cin >> hb >> wb;
    if (hs + 1 >= hb || ws + 1 >= wb)
    {
      cout << 0 << '\n';
    }
    else
    {
      //      sum of area of all rect | areas of rect      | areas of rect      | areas of rect
      //       with hi < hb, wi < wb  | hi <= hs, wi < wb  | hi < hb, wi <= ws  | hi <= hs, wi <= ws
      long long ans = prefix[hb - 1][wb - 1] - prefix[hs][wb - 1] - prefix[hb - 1][ws] + prefix[hs][ws];
      cout << ans << '\n';
    }
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