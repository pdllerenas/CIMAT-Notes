#include <iostream>
using namespace std;

void solve(int case_num)
{
  int N;
  cin >> N;

  // repetitions of the game
  int count = 0;
  // the winner is the last seat for N = 2^k - 1
  while ((N & (N + 1)) != 0)
  {
    count++;
    // decompose remaining players as 2^k + m, where m will give us the winning seat
    int greatest_pow2 = 1 << (31 - __builtin_clz(N));
    N = (N - greatest_pow2) * 2 + 1;
  }
  cout << "Case " << case_num << ": " << count << " " << N << "\n";
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  for (int i = 1; i <= t; i++)
  {
    solve(i);
  }

  return 0;
}