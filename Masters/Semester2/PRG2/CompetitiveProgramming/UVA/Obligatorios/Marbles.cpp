#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define BOARD_SIZE 105

// we will dynamically build this matrix,
// with each entry equalling its MEX score
int dp[BOARD_SIZE][BOARD_SIZE];

void preprocess()
{
  for (int i = 1; i < BOARD_SIZE; i++)
  {
    for (int j = 1; j < BOARD_SIZE; j++)
    {
      if (i == j)
        continue;

      // keeps track of the Grundy numbers reachable from (i,j)
      bool history[500] = {false};

      // up
      for (int k = 1; k < i; k++)
        if (i - k != j)
          history[dp[i - k][j]] = true;

      // left
      for (int k = 1; k < j; k++)
        if (j - k != i)
          history[dp[i][j - k]] = true;

      // diagonal
      for (int k = 1; k <= min(i - 1, j - 1); k++)
        history[dp[i - k][j - k]] = true;

      // minimum excluded
      int mex = 0;
      while (history[mex])
      {
        mex++;
      }
      dp[i][j] = mex;
    }
  }
}

void solve()
{
  int N;
  cin >> N;
  // by Sprague Grundy Theorem, position is won if xor sum is not equal to 0
  int xor_sum = 0;
  bool can_win_next = false;
  for (int i = 0; i < N; i++)
  {
    int l, c;
    cin >> l >> c;
    if (l == c)
    {
      can_win_next = true;
    }
    if (!can_win_next)
    {
      xor_sum ^= dp[l][c];
    }
  }

  if (xor_sum > 0 || can_win_next)
  {
    cout << "Y\n";
  }
  else
  {
    cout << "N\n";
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  preprocess();
  solve();

  return 0;
}