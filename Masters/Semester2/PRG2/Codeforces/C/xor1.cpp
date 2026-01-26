#include <bits/stdc++.h>
using namespace std;

void solution(const vector<int> &a, const vector<int> &b) {
  int a_init = 0, b_init = 0;
  int aji_swaps = 0, mai_swaps = 0;
  for (int i = 0; i < a.size(); i++) {
    a_init ^= a[i];
    b_init ^= b[i];
    if (i % 2) {
      aji_swaps += a[i] ^ b[i];
    } else {
      mai_swaps += a[i] ^ b[i];
    }
  }
  if (aji_swaps == mai_swaps) {
    cout << "Draw" << endl;
    return;
  }
  if (a_init < b_init) {
    if (aji_swaps > mai_swaps) {
      cout << "Ajisai" << endl;
    } else if (aji_swaps == mai_swaps) {
      cout << "Mai" << endl;
    }
  }
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;

  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> a, b;
    a.reserve(n);
    b.reserve(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    for (int i = 0; i < n; i++) {
      cin >> b[i];
    }
    solution(a, b);
  }

  return 0;
}
