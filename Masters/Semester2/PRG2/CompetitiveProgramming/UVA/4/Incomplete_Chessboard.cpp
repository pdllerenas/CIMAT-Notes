#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct State
{
  int x, y;
  int ply;
};

int dx[8] = {0, 0, -1, -1, -1, 1, 1, 1};
int dy[8] = {1, -1, -1, 1, 0, 1, -1, 0};

bool is_valid(int x, int y, int ox, int oy)
{
  return (x >= 1 && x <= 8 && y >= 1 && y <= 8 && (x != ox || y != oy));
}

void solve()
{
  int x0, y0;
  int xf, yf;
  int obs_x, obs_y;
  int count = 1;

  while (cin >> x0 >> y0 >> xf >> yf >> obs_x >> obs_y)
  {
    queue<State> q;
    // 1 more since input is 1-based
    bool visited[9][9] = {false};
    q.push({x0, y0, 0});
    visited[x0][y0] = true;
    while (!q.empty())
    {
      State curr = q.front();
      q.pop();
      // check all 8 moves
      if (curr.x == xf && curr.y == yf)
      {
        cout << "Case " << count << ": " << curr.ply << '\n';
        break;
      }
      for (int i = 0; i < 8; i++)
      {
        int next_x = curr.x + dx[i];
        int next_y = curr.y + dy[i];

        if (!is_valid(next_x, next_y, obs_x, obs_y) || visited[next_x][next_y])
          continue;

        visited[next_x][next_y] = true;
        q.push({next_x, next_y, curr.ply + 1});
      }
    }
    count++;
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}