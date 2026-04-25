#include <bits/stdc++.h>
using namespace std;

bool solution(vector<int> a, vector<int> b) {
  if (a.size() || b.size()) {
    if (a.front() >= b.front()) {
      pop_heap(b.begin(), b.end());
      b.pop_back();
      return solution(b, a);
    } else {
      pop_heap(b.begin(), b.end());
      int m = b.back();
      m -= a.front();
      return solution(b, a);
    }
  } else {
    return a.size() < b.size();
  }
}

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;

  while (t--) {
    int l_a, l_b;
    cin >> l_a >> l_b;
    vector<int> a(l_a), b(l_b);
    make_heap(a.begin(), a.end());
    make_heap(b.begin(), b.end());
    bool s = solution(a, b);
		cout << s << endl;
  }

  return 0;
}
