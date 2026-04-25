#include <iostream>
#include <map>
using namespace std;
int main()
{
  ios::sync_with_stdio(false);
  int nPattern;
  while (cin >> nPattern)
  {
    string s;
    map<string, int> counter;
    cin >> s;
    // vmax is the max number of repetitions of a pattern, imax the index of said pattern
    int vmax = 0, imax;
    // l is the last index for which we can get a pattern of length nPattern
    int l = s.size() - nPattern + 1;
    for (int i = 0; i < l; i++)
    {
      int v = counter[s.substr(i, nPattern)]++;
      if (v > vmax)
      {
        vmax = v;
        imax = i;
      }
    }
    cout << s.substr(imax, nPattern) << endl;
  }
  return 0;
}