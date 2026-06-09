#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// to keep track of the successfully matched
// pairs of brackets, but also consider
// those that where left closed or open,
// so if a range contains various nodes,
// we can calculate the possible resolution of
// unmatched pairs
struct Node
{
  int ans;
  int open;
  int closed;
};

class SegmentTree
{
private:
  string A;
  vector<Node> st;
  size_t n;

  int left(int p) const
  {
    return (p << 1);
  }
  int right(int p) const
  {
    return (p << 1) + 1;
  }

  // merges two nodes and solves any unmatched pairs
  Node merge(const Node &l, const Node &r) const
  {
    Node res;
    int matches = min(l.open, r.closed); // match left node open with right closed
    res.ans = l.ans + r.ans + 2 * (matches);
    res.open = l.open + r.open - matches;
    res.closed = l.closed + r.closed - matches;
    return res;
  }

  void build(int p, int L, int R)
  {
    if (L == R)
    {
      if (A[L] == '(')
      {
        st[p] = {0, 1, 0}; // 1 open
      }
      else
      {
        st[p] = {0, 0, 1}; // 1 closed
      }
    }
    else
    {
      int mid = L + (R - L) / 2;
      build(left(p), L, mid);
      build(right(p), mid + 1, R);

      st[p] = merge(st[left(p)], st[right(p)]);
    }
  }

  Node query(int p, int L, int R, int i, int j) const
  {
    if (i > R || j < L)
    {
      return {0, 0, 0};
    }

    if (L >= i && R <= j)
    {
      return st[p];
    }

    int mid = L + (R - L) / 2;

    Node p1 = query(left(p), L, mid, i, j);
    Node p2 = query(right(p), mid + 1, R, i, j);

    return merge(p1, p2);
  }

public:
  SegmentTree(const string &s) : A(s)
  {
    n = A.size();
    st.assign(4 * n, {0, 0, 0});
    build(1, 0, n - 1);
  }

  int query(int i, int j) const
  {
    return query(1, 0, n - 1, i, j).ans;
  }
};

void solve()
{
  string brackets;
  cin >> brackets;

  SegmentTree t(brackets);

  int m;
  cin >> m;

  for (int i = 0; i < m; i++)
  {
    int l, r;
    cin >> l >> r;
    cout << t.query(l - 1, r - 1) << '\n';
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}