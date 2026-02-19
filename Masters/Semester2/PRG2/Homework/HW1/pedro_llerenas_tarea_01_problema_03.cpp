#include <iostream>
#include <vector>

// Naïve approach
class FenwickTree
{
private:
  std::vector<long long> tree;

  void update(int index, long long value)
  {
    while (index < tree.size())
    {
      tree[index] += value;
      index += index & -index;
    }
  }


public:
  // default value of each element is 0
  FenwickTree(int size) : tree(size + 1, 0) {}

  // point query
  long long sum(int index)
  {
    long long s = 0;
    while (index > 0)
    {
      s += tree[index];
      index -= index & -index;
    }
    return s;
  }

  // range add: updates the range [l, r] by adding value to each element in that range
  void add(int l, int r, long long value)
  {
    update(l, value);
    if (r + 1 < tree.size())
      update(r + 1, -value);
  }

  // cap single index
  void cap(int index, long long value)
  {
    long long currentValue = sum(index);
    if (currentValue > value)
    {
      long long delta = value - currentValue;
      update(index, delta);
      if (index + 1 < tree.size())
        update(index + 1, -delta);
    }
  }

  // cap range [l, r] by setting each element to the minimum of its current value and the given value
  void cap(int l, int r, long long value)
  {
    for (int i = l; i <= r; ++i)
      cap(i, value);
  }

};

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, q;
  std::cin >> n >> q;

  FenwickTree ft(n);

  while (q--)
  {
    std::string op;
    std::cin >> op;

    if (op == "add")
    {
      int l, r;
      long long v;
      std::cin >> l >> r >> v;
      ft.add(l, r, v);
    }
    else if (op == "cap")
    {
      int l, r;
      long long v;
      std::cin >> l >> r >> v;
      ft.cap(l, r, v);
    }
    else if (op == "sum")
    {
      int i;
      std::cin >> i;
      std::cout << ft.sum(i) << "\n";
    }
  }

  return 0;
}