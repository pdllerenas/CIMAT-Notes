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

void radixSort(int k) {
	int m = max(300, n);
	memset(&freqs[0], 0, sizeof(freqs));

	// freq count
	for (int i = 0; i < n; i++) {
		if (i + k < n)
			freqs[rankarray[i+k]]++;
		else
			freqs[0]++;
	}

	// prefix sums
	int sfs = 0;
	for (int i = 0; i < m; i++) {
		int freq = freqs[i];
		freqs[i] = sfs;
		sfs += freq;
	}

	// temp array
	for (int i = 0; i < n; i++) {
		if (suffarray[i] + k < n) {
			tmpsuffarray[freqs[rankarray[suffarray[i] + k]]++] = suffarray[i];
		} else {
			tmpsuffarray[freqs[0]++] = suffarray[i];
		}
	}


	for (int i = 0; i < n; i++) {
		suffarray[i] = tmpsuffarray[i];
	}
}

void buildSuffArray() {
	for (int i = 0; i < n; i++) {
		rankarray[i] = s[i];
	}

	for (int i = 0; i < n; i++) {
		suffarray[i] = i;
	}

	for (unsigned int k = 1; k < n; k <<= 1) {
		radixSort(k);
		radixSort(0);

		int r = 0;
		tmprankarray[suffarray[0]] = r;
		for (int i = 1; i < n; i++ ) {
			if (rankarray[suffarray[i]] != rankarray[suffarray[i-1]]
			        || (suffarray[i] + k < n && rankarray[suffarray[i] + k] != rankarray[suffarray[i-1]+k])) {
				tmprankarray[suffarray[i]] = ++r;
			} else {
				tmprankarray[suffarray[i]] = r;
			}
		}
		for (int i = 0; i < n; i++) {
			rankarray[i] = tmprankarray[i];
		}
	}
}

void buildLCP() {
	phi[suffarray[0]] = -1;
	for (int i = 1; i < n; i++) {
		phi[suffarray[i]] = suffarray[i-1];
	}

	int l = 0;
	for (int i = 0; i < n; i++) {

		if (phi[i] == -1) {
			plcp[i] = 0;
			continue;
		}

		while (i + l < n && phi[i] + l < n && s[i + l] == s[phi[i] + l]) {
			l++;
		}

		plcp[i] = l;
		l = max(l - 1, 0);
	}

	for (int i = 0; i < n; i++) {
		lcp[i] = plcp[suffarray[i]];
	}
}

pair<int, int> lrs() {
	int ind = 0, lcpmax = -1;
	for (int i = 1; i < n; i++) {
		if (lcp[i] > lcpmax) {
			lcpmax = lcp[i];
			ind = i;
		}
	}
	return make_pair(lcpmax, ind);
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	
	int nCases;
	if (!(cin >> nCases)) return 0;

	for (int i = 0; i < nCases; i++) {
		cin >> s;

		int ogn = s.size();
		s += s;
		s += "|";
		n = s.size();

		buildSuffArray();
		int best;
		for (int j = 0; j < n; j++) {
			if (suffarray[j] < ogn) {
				best = suffarray[j];
				break;
			}
		}

		cout << best + 1 << endl;
	}

	return 0;
}
