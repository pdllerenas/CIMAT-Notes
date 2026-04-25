#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

vector<int> kmpPreprocess(const string &p)
{
  vector<int> resets(p.size() + 1);
  int i = 0, j = -1;
  resets[0] = -1;
  while (i < p.size())
  {
    while (j >= 0 && p[i] != p[j])
      j = resets[j];
    i++;
    j++;
    resets[i] = j;
  }
  return resets;
}

string shortestPalindrome(const string &s)
{
  if (s.empty()) return s;

  int n = s.length();
  string rev = s;
  reverse(rev.begin(), rev.end());

  string combined = rev + "|" + s;
  vector<int> resets = kmpPreprocess(combined);

  int pal_suffix_len = resets[combined.size()];
  string missing = s.substr(0, n - pal_suffix_len);

  reverse(missing.begin(), missing.end());
  return s + missing;
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string s;
  cin >> s;

  cout << shortestPalindrome(s) << endl;

  return 0;
}