#include <iostream>
#include <vector>

using namespace std;

vector<int> kmpPreprocess(const vector<int> &p)
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

int kmpMatch(const vector<int> &s, const vector<int> &pattern)
{
  if (pattern.empty())
    return s.size() + 1;

  int i = 0, j = 0, nc = 0;
  vector<int> resets = kmpPreprocess(pattern);
  while (i < s.size())
  {

    while (j >= 0 && s[i] != pattern[j])
      j = resets[j];
    i++;
    j++;
    if (j == pattern.size())
    {
      nc++;
      j = resets[j];
    }
  }
  return nc;
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, w;
  cin >> n >> w;
  vector<int> b_wall(n);
  vector<int> e_wall(w);
  for (int i = 0; i < n; ++i)
  {
    cin >> b_wall[i];
  }
  for (int i = 0; i < w; ++i)
  {
    cin >> e_wall[i];
  }

  if (n < w)
  {
    cout << 0 << "\n";
    return 0;
  }

  if (w == 1)
  {
    cout << n << "\n";
    return 0;
  }

  vector<int> b_diff(n - 1);
  for (int i = 0; i < n - 1; ++i)
  {
    b_diff[i] = b_wall[i + 1] - b_wall[i];
  }

  vector<int> e_diff(w - 1);
  for (int i = 0; i < w - 1; ++i)
  {
    e_diff[i] = e_wall[i + 1] - e_wall[i];
  }

  cout << kmpMatch(b_diff, e_diff) << endl;

  return 0;
}