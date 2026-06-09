#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

void solve()
{
  string line;
  while (getline(cin, line))
  {
    vector<unordered_set<string>> patterns(line.length() + 1);
    bool ok = true;
    
    // check the string character by character
    for (int i = 0; i < line.length(); i++)
    {
      bool violation = false;
      
      // for the current prefix ending at i, check all window sizes k
      for (int k = 1; k <= i + 1; k++)
      {
        // get substring of length k ending at i
        patterns[k].insert(line.substr(i - k + 1, k));
        
        // if the number of distinct patterns of length k exceeds k + 1
        if (patterns[k].size() > k + 1)
        {
          violation = true;
          break;
        }
      }
      
      if (violation)
      {
        cout << "NO:" << i + 1 << '\n'; 
        ok = false;
        break;
      }
    }
    
    if (ok)
    {
      cout << "YES\n";
    }
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}