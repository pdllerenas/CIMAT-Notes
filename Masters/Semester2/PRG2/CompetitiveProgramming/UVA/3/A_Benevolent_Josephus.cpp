#include <iostream>
using namespace std;

void solve()
{
  int N;
  while (cin >> N)
  {
    int remaining_players = N;
    int money_paid = 0;
    // the winner is the last seat for N = 2^k - 1, in which case we stop the game and pay everyone
    while ((remaining_players & (remaining_players + 1)) != 0)
    {
      // decompose remaining players as 2^k + m, where m will give us the winning seat
      int greatest_pow2 = 1 << (31 - __builtin_clz(remaining_players));
      int chosen = (remaining_players - greatest_pow2) * 2 + 1;

      money_paid += (remaining_players - chosen);
      remaining_players = chosen;
    }
    money_paid += remaining_players * 2;
    cout << money_paid << '\n';
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}