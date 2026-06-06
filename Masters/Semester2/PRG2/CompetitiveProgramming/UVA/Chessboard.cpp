#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;


/*
By Pick's theorem, we can calculate the area of a polygon with integer coordinate vertices
via 
A = I + B/2 - 1,
where I are the inside points, B are the boundary points.

In the chessboard, since the King visits every square once, all points are boundary points.
Therefore, B = n^2, I = 0.

And thus, the area enclosed by any king walk is n^2/2 - 1.

The grid made by the center of the squares is a smaller (n-1)^2 grid.
For each path, the king can either:
- go around the center of the square leaving it inside the king's polygon. (contributes 1) C_in
- go around the center of the square leaving it outside the king's polygon. (contributes 0) C_out
- go diagonally, leaving half of the square inside the polygon. (contributes 1/2) D

Since we want to maximize the polygons perimeter, we want to maximize the amount of diagonals

The total number of 1x1 squares is
C_in + C_out + D = (n-1)^2

The area enclosed is

C_in + 0.5 D = n^2/2 - 1

Thus

2 C_out + D = (n - 2)^2,

and maximizing for D (that is, set C_out = 0),
D = (n-2)^2

Thus, since there are n^2 moves and (n-2)^2 are diagonal moves (which have length sqrt(2)),
there are n^2 - (n-2)^2 = 4n - 4 straight moves,
and thus the total perimeter traces is

(4n-4) + (n-2)^2 * sqrt(2)
*/

void solve()
{
  int n;
  cin >> n;
  cout << fixed << setprecision(3);
  if (n == 1)
  {
    cout << 0.0f << '\n';
  }
  else
  {
    cout << (4 * n - 4) + (n - 2) * (n - 2) * sqrt(2) << '\n';
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