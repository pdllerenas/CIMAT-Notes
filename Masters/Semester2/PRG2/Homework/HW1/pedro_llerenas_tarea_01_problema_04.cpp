#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>

/*
  Sparse table for gcd queries of a static array
  (implementation is taken from code seen in class, with modifications to use gcd instead of min)
*/

class SparseTableGCD
{
private:
  int n_, K_;
  std::vector<std::vector<long long>> st_;
  std::vector<int> lg_;

public:
  explicit SparseTableGCD(const std::vector<long long> &a)
  {
    n_ = (int)a.size();
    K_ = (n_ ? (int)std::floor(std::log2(n_)) + 1 : 0);

    st_.assign(n_, std::vector<long long>(K_));
    lg_.assign(n_ + 1, 0);
    for (int i = 2; i <= n_; ++i)
      lg_[i] = lg_[i / 2] + 1;

    if (!n_)
      return;
    for (int i = 0; i < n_; ++i)
      st_[i][0] = a[i];

    for (int j = 1; j < K_; ++j)
      for (int i = 0; i + (1 << j) <= n_; ++i)
        st_[i][j] = std::gcd(st_[i][j - 1], st_[i + (1 << (j - 1))][j - 1]);
  }

  long long query(int l, int r) const
  {
    int j = lg_[r - l + 1];
    return std::gcd(st_[l][j], st_[r - (1 << j) + 1][j]);
  }

  size_t argmax_gcd_mod(int l, int G) const
  {
    if (query(l, l) % G != 0)
      return -1; // No valid R exists
    if (G <= 0)
      return -1;
    if (l < 0 || l >= n_)
      return -1;

    int ans = -1;
    int low = l, high = n_ - 1;
    while (low <= high)
    {
      int mid = low + (high - low) / 2;
      if (query(l, mid) % G == 0)
      {
        ans = mid;
        low = mid + 1;
      }
      else
      {
        high = mid - 1;
      }
    }
    return ans;
  }
};

int main()
{
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n;
  std::cin >> n;

  if (n <= 0)
  {
    std::cout << "Invalid array size." << std::endl;
    return 1;
  }

  std::vector<long long> inputArr(n);
  int i = 0;
  while (i < n)
  {
    std::cin >> inputArr[i];
    i++;
  }

  SparseTableGCD st(inputArr);
  int q;
  std::cin >> q;

  if (q <= 0)
  {
    std::cout << "Invalid number of queries." << std::endl;
    return 1;
  }

  while (q--)
  {
    int c, l, G;
    std::cin >> c >> l >> G;
    if (c == 1)
    {
      int result = st.query(l, G);
      std::cout << result << std::endl;
    }
    else if (c == 2)
    {
      int result = st.argmax_gcd_mod(l, G);
      if (result == -1)
      {
        std::cout << -1 << std::endl;
      }
      else
      {
        std::cout << result + 1 << std::endl; // +1 for 1-based index
      }
    }
    else
    {
      std::cout << "Invalid query type." << std::endl;
    }
  }
  return 0;
}