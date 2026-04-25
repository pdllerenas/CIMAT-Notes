#include <bits/stdc++.h>

using namespace std;

tuple<int, int, int> solution(int w, int h, int d, int N) {
  if (N == 1) {
    return tuple<int, int, int>(0, 0, 0);
  }
  int w_c = gcd(N, w);
  N /= w_c;
  int h_c = gcd(N, h);
  N /= h_c;
  int d_c = gcd(N, d);
  N /= d_c;

  if (N != 1) {
    return tuple<int, int, int>(-1, -1, -1);
  }
  return tuple<int, int, int>(w_c, h_c, d_c);
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int w, h, d;
  int N;
  cin >> w >> h >> d;
  cin >> N;
  tuple<int, int, int> s = solution(w, h, d, N);
  if (get<0>(s) == -1) {
    cout << "-1" << endl;
		return 0;
  }
  cout << get<0>(s) - 1 << ' ' << get<1>(s) - 1 << ' ' << get<2>(s) - 1 << endl;
  return 0;
}
