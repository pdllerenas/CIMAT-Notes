#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <string>

using namespace std;

#define SIZEMAX 250005

string s;
int freqs[SIZEMAX];
int suffarray[SIZEMAX];
int rankarray[SIZEMAX];
int tmprankarray[SIZEMAX];
int tmpsuffarray[SIZEMAX];
int phi[SIZEMAX];

int lcp[SIZEMAX];
int plcp[SIZEMAX];

int n;

int get_rank(int i)
{
  return i < n ? rankarray[i] : 0;
}

void radixSort(int k)
{
  int m = max(300, n + 2);
  memset(&freqs[0], 0, m * sizeof(int));

  for (int i = 0; i < n; i++)
  {
    freqs[get_rank(i + k)]++;
  }

  int sfs = 0;
  for (int i = 0; i < m; i++)
  {
    int freq = freqs[i];
    freqs[i] = sfs;
    sfs += freq;
  }

  for (int i = 0; i < n; i++)
  {
    int r = get_rank(suffarray[i] + k);
    tmpsuffarray[freqs[r]++] = suffarray[i];
  }

  for (int i = 0; i < n; i++)
  {
    suffarray[i] = tmpsuffarray[i];
  }
}

void buildLCP()
{
  phi[suffarray[0]] = -1;
  for (int i = 1; i < n; i++)
  {
    phi[suffarray[i]] = suffarray[i - 1];
  }

  int l = 0;
  for (int i = 0; i < n; i++)
  {
    if (phi[i] == -1)
    {
      plcp[i] = 0;
      continue;
    }

    while (i + l < n && phi[i] + l < n && s[i + l] == s[phi[i] + l])
    {
      l++;
    }

    plcp[i] = l;
    l = max(l - 1, 0);
  }

  for (int i = 0; i < n; i++)
  {
    lcp[i] = plcp[suffarray[i]];
  }
}

void buildSuffArray()
{
  for (int i = 0; i < n; i++)
  {
    rankarray[i] = (unsigned char)s[i];
  }

  for (int i = 0; i < n; i++)
  {
    suffarray[i] = i;
  }

  for (unsigned int k = 1; k < n; k <<= 1)
  {
    radixSort(k);
    radixSort(0);

    int r = 1;
    tmprankarray[suffarray[0]] = r;
    for (int i = 1; i < n; i++)
    {
      int curr_rank1 = get_rank(suffarray[i]);
      int curr_rank2 = get_rank(suffarray[i] + k);
      int prev_rank1 = get_rank(suffarray[i - 1]);
      int prev_rank2 = get_rank(suffarray[i - 1] + k);
      if (curr_rank1 != prev_rank1 || curr_rank2 != prev_rank2)
      {
        r++;
      }
      tmprankarray[suffarray[i]] = r;
    }

    for (int i = 0; i < n; i++)
    {
      rankarray[i] = tmprankarray[i];
    }

    if (r == n)
      break;
  }
}

struct Match
{
  int len;
  int jcn_pos;

  bool operator<(const Match &other) const
  {
    if (len != other.len)
      return len > other.len;       // Descending by length
    return jcn_pos < other.jcn_pos; // Ascending by JCN position
  }
};

void solve()
{
  int k;
  int test_case = 1;

  while (cin >> k && k != 0)
  {
    string line;
    string tdp = "";

    while (getline(cin, line) && line.find("BEGIN TDP CODEBASE") == string::npos) {}
    while (getline(cin, line) && line.find("END TDP CODEBASE") == string::npos)
    {
      if (!line.empty() && line.back() == '\r') line.pop_back();
      tdp += line + '\n';
    }

    string jcn = "";
    
    while (getline(cin, line) && line.find("BEGIN JCN CODEBASE") == string::npos) {}
    while (getline(cin, line) && line.find("END JCN CODEBASE") == string::npos)
    {
      if (!line.empty() && line.back() == '\r') line.pop_back();
      jcn += line + '\n';
    }

    s = tdp + char(1) + jcn;
    n = s.size();

    buildSuffArray();
    buildLCP();

    int tdp_len = tdp.size();
    vector<int> max_len(n, 0);

    // Forward pass
    int running_lcp = 0;
    for (int i = 0; i < n; i++)
    {
      running_lcp = min(running_lcp, lcp[i]);
      if (suffarray[i] < tdp_len)
      {
        running_lcp = 1e9;
      }
      else if (suffarray[i] > tdp_len)
      {
        max_len[suffarray[i]] = max(max_len[suffarray[i]], running_lcp);
      }
    }

    // Backward pass
    running_lcp = 0;
    for (int i = n - 1; i >= 0; i--)
    {
      if (suffarray[i] < tdp_len)
      {
        running_lcp = 1e9;
      }
      else if (suffarray[i] > tdp_len)
      {
        max_len[suffarray[i]] = max(max_len[suffarray[i]], running_lcp);
      }
      running_lcp = min(running_lcp, lcp[i]);
    }

    vector<Match> candidates;
    for (int i = tdp_len + 1; i < n; i++)
    {
      if (max_len[i] > 0)
      {
        candidates.push_back({max_len[i], i - (tdp_len + 1)});
      }
    }

    sort(candidates.begin(), candidates.end());

    vector<string> accepted_strings; 

    if (test_case > 1)
    {
      cout << "\n";
    }

    cout << "CASE " << test_case++ << "\n";

    int count = 1;
    for (const auto &match : candidates)
    {
      if (accepted_strings.size() == (size_t)k)
        break;

      string seg = jcn.substr(match.jcn_pos, match.len);
      bool is_contained = false;

      for (const auto &acc_str : accepted_strings)
      {
        if (acc_str.find(seg) != string::npos)
        {
          is_contained = true;
          break;
        }
      }

      if (!is_contained)
      {
        accepted_strings.push_back(seg); 
        
        cout << "INFRINGING SEGMENT " << count << " LENGTH " << match.len
             << " POSITION " << match.jcn_pos << "\n";
             
        cout << seg;
        
        if (!seg.empty() && seg.back() != '\n') {
            cout << "\n";
        }
        
        count++;
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