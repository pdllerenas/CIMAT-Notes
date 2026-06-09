#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> parent_node;
vector<int> set_rank;

int find_set(int v)
{
  if (v == parent_node[v])
    return v;
  return parent_node[v] = find_set(parent_node[v]);
}

void union_sets(int a, int b)
{
  a = find_set(a);
  b = find_set(b);
  if (a != b)
  {
    if (set_rank[a] < set_rank[b])
    {
      swap(a, b);
    }
    parent_node[b] = a;
    if (set_rank[a] == set_rank[b])
    {
      set_rank[a]++;
    }
  }
}

void solve()
{
  vector<string> dict;
  string s;

  while (cin >> s && s != "--")
  {
    dict.push_back(s);
  }

  // sort and remove duplicates
  sort(dict.begin(), dict.end());
  dict.erase(unique(dict.begin(), dict.end()), dict.end());

  int n = dict.size();
  parent_node.resize(n);
  set_rank.assign(n, 0);
  for (int i = 0; i < n; ++i)
  {
    parent_node[i] = i;
  }

  // build connectvity graph
  for (int i = 0; i < n; ++i)
  {
    string temp = dict[i];

    for (size_t j = 0; j < temp.length(); ++j)
    {
      char orig = temp[j];

      // check for chars only greater then the one being changed.
      // this can be done
      for (char c = orig + 1; c <= 'z'; ++c)
      {
        temp[j] = c;

        // Binary search for the modified word
        auto it = lower_bound(dict.begin(), dict.end(), temp);
        if (it != dict.end() && *it == temp)
        {
          int sibling_idx = distance(dict.begin(), it);
          union_sets(i, sibling_idx);
        }
      }
      // only one letter at a time
      temp[j] = orig;
    }
  }

  string w1, w2;
  while (cin >> w1 >> w2)
  {
    // if diff length, impossible
    if (w1.length() != w2.length())
    {
      cout << "No\n";
      continue;
    }

    // make suer both words actually exist in the dictionary
    auto it1 = lower_bound(dict.begin(), dict.end(), w1);
    if (it1 == dict.end() || *it1 != w1)
    {
      cout << "No\n";
      continue;
    }
    int id1 = distance(dict.begin(), it1);

    auto it2 = lower_bound(dict.begin(), dict.end(), w2);
    if (it2 == dict.end() || *it2 != w2)
    {
      cout << "No\n";
      continue;
    }
    int id2 = distance(dict.begin(), it2);

    // check if they belong to the same component
    if (find_set(id1) == find_set(id2))
    {
      cout << "Yes\n";
    }
    else
    {
      cout << "No\n";
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