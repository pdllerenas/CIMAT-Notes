#include <bits/stdc++.h>

using namespace std;

// best avg is the max value, since
// a <= b --> a <= a+b/2 <= b
int solution(const vector<int> &a) {
  int max_val = 0;
  for (int v : a) {
    if (v > max_val) {
      max_val = v;
    }
  }
  return max_val;
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
