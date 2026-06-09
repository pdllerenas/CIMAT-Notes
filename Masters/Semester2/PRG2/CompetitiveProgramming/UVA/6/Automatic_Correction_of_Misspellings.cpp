#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_set>

using namespace std;

void solve()
{
  int n;
  while (cin >> n)
  {

    unordered_set<string> dict(n);
    vector<string> dictionary(n);
    for (int i = 0; i < n; i++)
    {
      string word;
      cin >> word;
      dict.insert(word);
      dictionary[i] = word;
    }

    int q;
    cin >> q;
    for (int i = 0; i < q; i++)
    {
      string word;
      cin >> word;

      if (dict.find(word) != dict.end())
      {
        cout << word << " is correct\n";
        continue;
      }

      bool found = false;

      for (int j = 0; j < n; j++)
      {
        string &dict_word = dictionary[j];

        // prune lengths
        if (abs((int)word.size() - (int)dict_word.size()) > 1)
          continue;

        int mismatches = 0;

        // equal lengths
        if (word.size() == dict_word.size())
        {
          for (size_t k = 0; k < word.size(); k++)
          {
            if (word[k] != dict_word[k])
            {
              // check for adjacent swap
              if (k + 1 < word.size() && word[k] == dict_word[k + 1] && word[k + 1] == dict_word[k])
              {
                k++; // skip the next character since it's part of this swap
                mismatches++;
              }
              else
              {
                mismatches++;
              }
            }
          }

          if (mismatches == 1)
          {
            cout << word << " is a misspelling of " << dict_word << '\n';
            found = true;
            break;
          }
        }

        // lengths differ by 1 (insertion / deletion)
        else if (abs((int)word.size() - (int)dict_word.size()) == 1)
        {
          int w1 = 0, w2 = 0;
          while (w1 < word.size() && w2 < dict_word.size())
          {
            if (word[w1] != dict_word[w2])
            {
              mismatches++;

              if (mismatches > 1)
                break;

              if (word.size() < dict_word.size())
                w2++;
              else
                w1++;
            }
            else
            {
              w1++;
              w2++;
            }
          }

          // if mismatches == 0 at the end of the loop, it means the differing
          // character was exactly the very last character of the longer string
          if (mismatches <= 1)
          {
            cout << word << " is a misspelling of " << dict_word << '\n';
            found = true;
            break;
          }
        }
      }

      if (!found)
      {
        cout << word << " is unknown\n";
      }
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