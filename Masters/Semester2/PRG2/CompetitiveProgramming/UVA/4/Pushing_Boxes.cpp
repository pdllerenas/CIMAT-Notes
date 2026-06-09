#include <queue>
#include <vector>
#include <iostream>

using namespace std;

struct State
{
  // we store both box coord and worker coords, as we may be pushing the
  // box to a seen position, but the worker might be somewhere else,
  // and viceversa
  int bx, by; // box coords
  int wx, wy; // worker coords
  int pushes;
  int walks;
  string path;

  // we compare two states by first
  // comparing the amount of pushes,
  // then the number of walks.
  // since we are minimizing costs,
  // we want the smaller element to be chosen
  bool operator>(const State &other) const
  {
    if (pushes != other.pushes)
    {
      return pushes > other.pushes;
    }
    return walks > other.walks;
  }
};

void solve()
{
  int r, c;
  int maze_count = 1;

  while (cin >> r >> c && (r != 0 || c != 0))
  {
    int bx0, by0, wx0, wy0, tx0, ty0;
    bool is_wall[20][20] = {false};

    for (int row = 0; row < r; row++)
    {
      string line;
      cin >> line;
      for (int col = 0; col < c; col++)
      {
        if (line[col] == '#')
        {
          is_wall[row][col] = true;
        }
        else
        {
          is_wall[row][col] = false;

          if (line[col] == 'S')
          {
            wx0 = row;
            wy0 = col;
          }
          else if (line[col] == 'B')
          {
            bx0 = row;
            by0 = col;
          }
          else if (line[col] == 'T')
          {
            tx0 = row;
            ty0 = col;
          }
        }
      }
    }
    // type is State, container is a vector, and compare using defined > in struct
    priority_queue<State, vector<State>, greater<State>> pq;
    bool visited[20][20][20][20] = {false};

    pq.push({bx0, by0, wx0, wy0, 0, 0, ""});

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char w_dir[] = {'n', 's', 'w', 'e'};
    char b_dir[] = {'N', 'S', 'W', 'E'};

    bool found = false;
    string ans = "";

    // Dijkstra
    while (!pq.empty())
    {
      State curr = pq.top();
      pq.pop();

      // skip if a shorter path has been found
      if (visited[curr.bx][curr.by][curr.wx][curr.wy])
        continue;
      visited[curr.bx][curr.by][curr.wx][curr.wy] = true;

      // check if box is in target
      if (curr.bx == tx0 && curr.by == ty0)
      {
        ans = curr.path;
        found = true;
        break;
      }

      // generate all 4 plausible moves
      for (int i = 0; i < 4; i++)
      {
        int next_wx = curr.wx + dr[i];
        int next_wy = curr.wy + dc[i];

        // check if worker is inside map limits
        if (next_wx < 0 || next_wx >= r || next_wy < 0 || next_wy >= c)
          continue;

        // skip if blocked by wall
        if (is_wall[next_wx][next_wy])
          continue;

        // if we are about to push the box
        if (next_wx == curr.bx && next_wy == curr.by)
        {
          int next_bx = curr.bx + dr[i];
          int next_by = curr.by + dc[i];

          // check if box is within map limits
          if (next_bx < 0 || next_bx >= r || next_by < 0 || next_by >= c)
            continue;

          // skip if we are pushing it into a wall tile
          if (is_wall[next_bx][next_by])
            continue;

          if (!visited[next_bx][next_by][next_wx][next_wy])
          {
            pq.push({next_bx, next_by, next_wx, next_wy, curr.pushes + 1, curr.walks + 1, curr.path + b_dir[i]});
          }
        }
        else
        { // if we are not pushing the box
          if (!visited[curr.bx][curr.by][next_wx][next_wy])
          {
            pq.push({curr.bx, curr.by, next_wx, next_wy, curr.pushes, curr.walks + 1, curr.path + w_dir[i]});
          }
        }
      }
    }

    cout << "Maze #" << maze_count << '\n';
    if (found)
    {
      cout << ans << '\n';
    }
    else
    {
      cout << "Impossible.\n";
    }
    cout << '\n';

    maze_count++;
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}