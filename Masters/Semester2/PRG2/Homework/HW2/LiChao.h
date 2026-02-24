#include <vector>
using namespace std;

using ll = long long;

struct Line
{
  ll m, b;
  ll eval(ll x) const
  {
    return m * x + b;
  }
};

class LiChaoTree
{
private:
  static const ll INF = 4e18;
  ll L_, R_;
  vector<Line> tree_;

  void add_line(Line nw, ll v, ll l, ll r)
  {
    ll m = (l + r) / 2;
    bool lef = nw.eval(l) < tree_[v].eval(l);
    bool mid = nw.eval(m) < tree_[v].eval(m);

    if (mid)
      swap(tree_[v], nw);

    if (r - l == 1)
      return;
    else if (lef != mid)
      add_line(nw, 2 * v, l, m);
    else
      add_line(nw, 2 * v + 1, m, r);
  }

  ll get(ll x, ll v, ll l, ll r) const
  {
    ll m = (l + r) / 2;
    if (r - l == 1)
      return tree_[v].eval(x);

    if (x < m)
      return min(tree_[v].eval(x), get(x, 2 * v, l, m));
    else
      return min(tree_[v].eval(x), get(x, 2 * v + 1, m, r));
  }

public:
  LiChaoTree(ll xL, ll xR) : L_(xL), R_(xR)
  {
    tree_.assign(4 * (R_ - L_), {0, INF});
  }

  void add_line(ll m, ll b)
  {
    add_line({m, b}, 1, L_, R_);
  }

  ll query(ll x) const
  {
    return get(x, 1, L_, R_);
  }
};