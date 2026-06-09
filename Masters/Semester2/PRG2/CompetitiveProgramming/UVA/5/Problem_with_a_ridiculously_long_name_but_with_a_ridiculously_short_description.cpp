#include <iostream>
#include <string>
using namespace std;

/*
Since we are doing modulo 100, we only need to look at the last 2 digits of 66^n

Now, we can look at the sequence of these digits:

n - ans
0 - 1
1 - 66
2 - 56
3 - 96
4 - 36
5 - 76
6 - 16
7 - 56

So we only need to do (n - 2) mod 5.
Since n is huge, we can simplify this to only looking at the last digit of n since n is base 10 and the mod is 5.
To prevent negative module results, we add 5, so we have
(last_digit(n) - 2 + 5) mod 5
*/

void solve()
{
  string big_n;
  cin >> big_n;

  if (big_n == "0")
  {
    cout << 1 << '\n';
  }
  else if (big_n == "1")
  {
    cout << 66 << '\n';
  }
  else
  {
    int last_digit = big_n.back() - '0';
    int c = (last_digit - 2 + 5) % 5;
    if (c == 0)
      cout << 56;
    else if (c == 1)
      cout << 96;
    else if (c == 2)
      cout << 36;
    else if (c == 3)
      cout << 76;
    else if (c == 4)
      cout << 16;
    cout << '\n';
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