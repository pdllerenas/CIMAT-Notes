#include <iostream>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

enum Move
{
  NONE = 0,
  K = 1,
  B = 2,
  T = 3,
};

// regular knight move displacement pairs
int dx_K[8] = {1, 1, -1, -1, 2, 2, -2, -2};
int dy_K[8] = {2, -2, 2, -2, 1, -1, 1, -1};

// 'bishop' moves displacement pairs
int dx_B[] = {2, 2, -2, -2};
int dy_B[] = {2, -2, 2, -2};

struct State
{
  int x, y;
  int ply;
  Move lastMove;
};

bool is_obstacle[50][50];

// check if move can be made
bool is_valid(int x, int y, int n)
{
  return (x >= 1 && x <= 2 * n && y >= 1 && y <= 2 * n && !is_obstacle[x][y]);
}

// returns the amount of steps (ply) to reach xf, yf
// n is the size of the board
int bfs(int x0, int y0, int xf, int yf, int n)
{
  // since n <= 20, size of board is 2 * n <= 40
  bool visited[50][50][4] = {false};
  queue<State> q;

  q.push({x0, y0, 0, NONE});
  visited[x0][y0][NONE] = true;

  while (!q.empty())
  {
    State curr = q.front();
    q.pop();

    if (curr.x == xf && curr.y == yf)
    {
      return curr.ply;
    }

    // check if we can make the predefined moves
    for (int m = 1; m <= 3; m++)
    {
      // cant make the same move twice
      if (m == curr.lastMove)
        continue;

      int next_x;
      int next_y;
      if (m == K)
      {
        for (int i = 0; i < 8; i++)
        {
          next_x = curr.x + dx_K[i];
          next_y = curr.y + dy_K[i];
          if (!visited[next_x][next_y][K] && is_valid(next_x, next_y, n))
          {
            visited[next_x][next_y][K] = true;
            q.push({next_x, next_y, curr.ply + 1, K});
          }
        }
      }

      else if (m == B)
      {
        for (int i = 0; i < 4; i++)
        {
          next_x = curr.x + dx_B[i];
          next_y = curr.y + dy_B[i];
          if (!visited[next_x][next_y][B] && is_valid(next_x, next_y, n))
          {
            visited[next_x][next_y][B] = true;
            q.push({next_x, next_y, curr.ply + 1, B});
          }
        }
      }

      else if (m == T)
      {
        // reflect on y axis
        next_x = curr.x;
        next_y = 2 * n + 1 - curr.y;
        if (!visited[next_x][next_y][T] && is_valid(next_x, next_y, n))
        {
          visited[next_x][next_y][T] = true;
          q.push({next_x, next_y, curr.ply + 1, T});
        }

        // reflect on x axis
        next_x = 2 * n + 1 - curr.x;
        next_y = curr.y;
        if (!visited[next_x][next_y][T] && is_valid(next_x, next_y, n))
        {
          visited[next_x][next_y][T] = true;
          q.push({next_x, next_y, curr.ply + 1, T});
        }
      }
      else
      {
        continue;
      }
    }
  }
  return -1;
}

void solve()
{
  int N;
  while (cin >> N && N > 0)
  {
    memset(is_obstacle, 0, sizeof(is_obstacle));
    int xo, yo;
    int xf, yf;
    cin >> xo >> yo >> xf >> yf;

    int ob_x, ob_y;
    while (cin >> ob_x >> ob_y && ob_x != 0 && ob_y != 0)
    {
      is_obstacle[ob_x][ob_y] = 1;
    }

    int shortest = bfs(xo, yo, xf, yf, N);
    if (shortest == -1)
    {
      cout << "Solution doesn't exist\n";
    }
    else
    {
      cout << "Result : " << shortest << '\n';
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