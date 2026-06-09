#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

void solve()
{
  int N, M;
  cin >> N >> M;
  int matches = 1 << N;
  vector<bool> playing(matches, true);
  int walk_outs = 0;

  while (M--)
  {
    int not_playing;
    cin >> not_playing;
    playing[not_playing - 1] = false;
  }

  while (N--) {
    for (int i = 0; i < matches; i += 2) {
      if (playing[i] ^ playing[i + 1]) {
        walk_outs++;
        playing[i / 2] = true;
      } else if (playing[i] & playing[i+1]) {
        playing[i / 2] = true;
      } else {
        playing[i / 2] = false;
      }
    }
    matches >>= 1;
  }
  cout << walk_outs << '\n';
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