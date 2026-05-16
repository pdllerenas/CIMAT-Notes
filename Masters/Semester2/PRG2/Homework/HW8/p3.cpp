#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string s;
  if (!getline(cin, s))
    return 0;

  const long long p = 31;
  const long long m = 1000000007;
  int n = s.size();

  // itereate over all string lengths
  for (int len = 1; len <= n; ++len)
  {
    unordered_map<long long, int> freq;
    long long current_hash = 0;
    long long p_pow = 1;

    // hash
    for (int i = 0; i < len; ++i)
    {
      current_hash = (current_hash * p + s[i]) % m;
      if (i < len - 1)
      {
        p_pow = (p_pow * p) % m;
      }
    }

    freq[current_hash]++;
    int max_freq = 1;

    // sliding window
    for (int i = len; i < n; ++i)
    {
      // remove previous char
      long long remove_term = (s[i - len] * p_pow) % m;
      current_hash = (current_hash - remove_term + m) % m;

      current_hash = (current_hash * p + s[i]) % m;

      freq[current_hash]++;
      if (freq[current_hash] > max_freq)
      {
        max_freq = freq[current_hash];
      }
    }

    // print result or stop loop, as bigger lengths will not have repetitions (by monotonicity)
    if (max_freq > 1)
    {
      cout << max_freq << "\n";
    }
    else
    {
      // if max_freq is 1, there are no repetitions
      break;
    }
  }

  return 0;
}