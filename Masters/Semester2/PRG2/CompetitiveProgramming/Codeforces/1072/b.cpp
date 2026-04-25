#include <bits/stdc++.h>
using namespace std;

void solution(int s, int k, int m) {
  if (m < k) {
    cout << max(s - m, 0) << endl;
    return;
  }

  if (k < s) {
    int stage = (m / k);
    if (stage % 2) {
      cout << s - (m % k) << endl;
      return;
    }
    if (m == k) {
      cout << k << endl;
    } else {
      cout << k % m << endl;
    }
  }
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int s, k, m;
    cin >> s >> k >> m;
    solution(s, k, m);
  }

  return 0;
}
