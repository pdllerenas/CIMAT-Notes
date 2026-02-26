#undef _GLIBCXX_DEBUG

#include <bits/stdc++.h>

using namespace std;
using ll = long long;
const ll INF = 4e18;

struct Line
{
  ll m, b;
  ll eval(ll x) const
  {
    return m * x + b;
  }
};

struct Node
{
  Line line;
  Node *left = nullptr;
  Node *right = nullptr;

  Node(Line l) : line(l) {}
};

class LiChaoTree
{
private:
  ll L, R;
  Node *root;

  /*
  nw: Line object to add
  l: left endpoint
  r: right endpoint
  node: reference [l,r] segment node
  */
  void add_line(Line nw, ll l, ll r, Node *&node)
  {
    // when segment has no assigned lines
    if (!node)
    {
      node = new Node(nw);
      return;
    }

    ll mid = (l + r) >> 1;

    bool left = nw.eval(l) <= node->line.eval(l);      // new node wins left side
    bool middle = nw.eval(mid) < node->line.eval(mid); // new node wins mid point

    if (middle)
      swap(node->line, nw);

    if (r - l == 1) // leaf
      return;
    else if (left != middle) // go to left subtree
      add_line(nw, l, mid, node->left);
    else // go to right subtree
      add_line(nw, mid, r, node->right);
  }

  ll query(ll x, ll l, ll r, Node *node) const
  {
    if (!node)
      return INF;

    ll res = node->line.eval(x);
    if (r - l == 1) // leaf
      return res;

    ll mid = (l + r) >> 1;

    if (x <= mid && node->left) // left subtree
      return min(res, query(x, l, mid, node->left));
    if (x > mid && node->right) // right subtree
      return min(res, query(x, mid, r, node->right));

    return res;
  }

public:
  LiChaoTree(ll l, ll r) : L(l), R(r), root(nullptr) {}

  void add_line(ll m, ll b)
  {
    Line nw{m, b};
    add_line(nw, L, R, root);
  }

  ll query(ll x) const
  {
    return query(x, L, R, root);
  }
};

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(0);

  int N, K;
  cin >> N >> K;
  vector<ll> A(N);
  for (int i = 0; i < N; i++)
  {
    cin >> A[i];
  }
  vector<ll> prev(N + 1, INF), curr(N + 1, INF);

  prev[0] = 0;

  for (int k = 1; k <= K; k++)
  {
    LiChaoTree lct(0, 1 + 1e6);
    fill(curr.begin(), curr.end(), INF);

    if (prev[0] != INF)
    {
      ll m = -2LL * A[0];
      ll b = prev[0] + A[0] * A[0];
      lct.add_line(m, b);
    }
    for (int i = 1; i <= N; i++)
    {
      ll best = lct.query(A[i - 1]);
      curr[i] = 1LL * A[i - 1] * A[i - 1] + best;

      if (i < N && prev[i] != INF)
      {
        ll m = -2LL * A[i];
        ll b = prev[i] + 1LL * A[i] * A[i];
        lct.add_line(m, b);
      }
    }
    prev = curr;
  }
  cout << prev[N] << "\n";

  return 0;
}