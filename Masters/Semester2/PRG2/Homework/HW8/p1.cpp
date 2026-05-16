#include <iostream>
#define INF 1e3

using namespace std;
int values[102][102];

string backtrack(const string &s1, const string &s2)
{
  int i = s1.size();
  int j = s2.size();
  string res = "";

  while (i > 0 || j > 0)
  {
    // if best is diagonal (equal or replace)
    if (i > 0 && j > 0 && values[i][j] == values[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1))
    {
      if (s1[i - 1] == s2[j - 1])
        res = "Match " + res;
      else
        res = "Substitute " + res;
      i--;
      j--;
    }
    // best is transpose
    else if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1] && values[i][j] == values[i - 2][j - 2] + 1)
    {
      res = "Transpose " + res;
      i -= 2;
      j -= 2;
    }
    // best is to delete
    else if (i > 0 && values[i][j] == values[i - 1][j] + 1)
    {
      res = "Delete " + res;
      i--;
    }
    // best is to insert
    else if (j > 0 && values[i][j] == values[i][j - 1] + 1)
    {
      res = "Insert " + res;
      j--;
    }
  }
  return res;
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string s1, s2;
  int nCase = 1;
  values[0][0] = 0;
  // cost of skipping is 1
  for (int i = 1; i < 102; i++)
  {
    values[i][0] = i;
    values[0][i] = i;
  }
  while (getline(cin, s1) && s1[0] != '#')
  {
    getline(cin, s2);
    for (int i = 1; i <= s1.size(); i++)
      for (int j = 1; j <= s2.size(); j++)
      {
        int remove = values[i - 1][j] + 1;
        int insert = values[i][j - 1] + 1;
        int sub = values[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1);
        int transpose = INF;
        if (i > 1 && j > 1)
        {
          bool can_transpose = (s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]);
          if (can_transpose)
          {
            transpose = values[i - 2][j - 2] + 1;
          }
        }
        values[i][j] = min(remove, min(insert, min(sub, transpose)));
      }
    cout << "Strings: " << s1 << " -> " << s2 << endl;
    cout << "Damerau-Levenshtein distance: " << values[s1.size()][s2.size()] << endl;
    cout << "Optimal Sequence: " << backtrack(s1, s2) << endl;
    cout << "-----------------------------------" << endl;
  }
  return 0;
}