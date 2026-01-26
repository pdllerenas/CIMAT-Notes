#include <bits/stdc++.h>

using namespace std;

int solution(const vector<int> &a) {
  int n_count = 0;
  int z_count = 0;
  for (int v : a) {
    if (v == -1) {
      n_count++;
    } else if (v == 0) {
      z_count++;
    }
  }
  return z_count + 2 * (n_count & 1);
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;

  while (t--) {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a)
      cin >> x;
    cout << solution(a) << endl;
  }
  return 0;
}
