#include <string>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define DICT_SIZE 26

// dictionary provided by problem on which
// chars are considered good
bool good_chars[DICT_SIZE] = {0};

// we use tries to solve the problem. We note that once a substring is bad,
// anything added to it is bad. That is, we have a monotonic property.
// Similarly, if a substring is good, its substrings are also good.
struct trieNode
{
  int children[DICT_SIZE];

  trieNode()
  {
    fill(begin(children), end(children), -1);
  }
};

class trie
{
  vector<struct trieNode> nodes;

public:
  trie() : nodes(1)
  {
  }

  // inserts a string into the trie, and k is the max number of bad chars allowed
  void insert(const string &s, int k)
  {
    int n = 0;
    int bad_chars = 0;
    for (char ch : s)
    {
      int c = ch - 'a';
      if (good_chars[c] == 0)
      {
        bad_chars++;
      }

      // if we get to the point where the substring is bad, stop and do not include into the trie.
      // We stop due to the monotonic property of the problem
      if (bad_chars > k)
      {
        break;
      }

      // if word is still good, do the usual updates
      if (nodes[n].children[c] == -1)
      {
        nodes[n].children[c] = nodes.size();
        nodes.push_back(trieNode());
      }
      n = nodes[n].children[c];
    }
  }

  // getter for the number of good words. Note that it is equal to the size
  // (minus 1, since we start with a trivial node)
  // because we broke the cycle when we encountered a bad word
  int getGoodWords() const
  {
    return nodes.size() - 1;
  }
};

void solve()
{
  string s;
  cin >> s;

  trie t;

  string gb;
  cin >> gb;

  int i = 0;
  for (char c : gb)
  {
    if (c == '0')
    {
      good_chars[i] = 0;
    }
    else
    {
      good_chars[i] = 1;
    }
    i++;
  }
  int k;
  cin >> k;

  // since we need to check all substrings, we go for all suffixes,
  // as the trie covers the prefixes of each inserted string
  for (int i = 0; i < s.size(); i++)
    t.insert(s.substr(i), k);

  cout << t.getGoodWords() << endl;
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}