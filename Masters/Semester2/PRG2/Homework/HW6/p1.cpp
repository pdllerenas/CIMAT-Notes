#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

vector<int> kmpPreprocess(const string &p)
{
  int i = 0, j = -1;
  vector<int> resets(p.length() + 1);
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

int distinctPatterns(const string &s)
{
  int i = 0, nc = 0;
  vector<string> patterns;
  while (i < s.size())
  {
    string prefix = s.substr(0, i + 1);
    reverse(prefix.begin(), prefix.end());
    vector<int> resets = kmpPreprocess(prefix);

    int max_lps = 0;
    for (int val : resets)
    {
      if (val > max_lps)
      {
        max_lps = val;
      }
    }
    nc += (i + 1) - max_lps;
    i++;
  }
  return nc;
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string s;
  cin >> s;

  cout << distinctPatterns(s) << endl;

  return 0;
}