#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>

using namespace std;

#define SIZEMAX 20001

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
	memset(&freqs[0], 0, sizeof(freqs));

	// freq count
	for (int i = 0; i < n; i++)
	{
		freqs[get_rank(i + k)]++;
	}

	// prefix sums
	int sfs = 0;
	for (int i = 0; i < m; i++)
	{
		int freq = freqs[i];
		freqs[i] = sfs;
		sfs += freq;
	}

	// temp array
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
		rankarray[i] = s[i];
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

void solve()
{
	int k;
	cin >> k;

	string ws;
	cin >> ws;

	string line;
	string tdp = "";
	if (getline(cin, line) && line == "BEGIN TDP CODEBASE")
	{
		while (getline(cin, line) && line != "END TDP CODEBASE")
		{
			tdp += line;
			tdp += '\n';
		}
	}

	string jcn = "";
	if (getline(cin, line) && line == "BEGIN JCN CODEBASE")
	{
		while (getline(cin, line) && line != "END JCN CODEBASE")
		{
			jcn += line;
			jcn += '\n';
		}
	}

	s = tdp + '|' + jcn;
	n = s.size();

	buildSuffArray();
	buildLCP();

	sort(lcp, cmp_less);

	int tdp_len = tdp.size();
	vector<int> max_len(n, 0); // tores the maximum match length for each index in JCN

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

	struct Match
	{
		int len;
		int jcn_pos;

		bool operator<(const Match &other) const
		{
			if (len != other.len)
				return len > other.len;				// Descending by length
			return jcn_pos < other.jcn_pos; // Ascending by JCN position
		}
	};

	vector<Match> candidates;
	for (int i = tdp_len + 1; i < n; i++)
	{
		if (max_len[i] > 0)
		{
			// The original JCN string starts after tdp_len + 1 (because of the '|')
			candidates.push_back({max_len[i], i - (tdp_len + 1)});
		}
	}

	sort(candidates.begin(), candidates.end());

	vector<Match> accepted;

	int count = 1;
	for (const auto &match : candidates)
	{
		if (accepted.size() == k)
			break; // We found enough matches

		int start = match.jcn_pos;
		int end = match.jcn_pos + match.len - 1;
		bool is_contained = false;

		// Check against all previously accepted matches
		for (const auto &acc : accepted)
		{
			int acc_start = acc.jcn_pos;
			int acc_end = acc.jcn_pos + acc.len - 1;

			if (start >= acc_start && end <= acc_end)
			{
				is_contained = true;
				break;
			}
		}

		if (!is_contained)
		{
			cout << "CASE " << count << '\n';
			accepted.push_back(match);
			cout << "INFRINGING SEGMENT OF LENGTH " << match.len
					 << " AT JCN POSITION " << match.jcn_pos << "\n"; // Example output
			count++;
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
