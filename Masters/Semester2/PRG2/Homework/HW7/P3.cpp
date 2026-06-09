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

string inverseBWT(const string &bwt) {
    int n = bwt.size();
    
    vector<pair<char, int>> F(n);
    for (int i = 0; i < n; i++) {
        F[i] = {bwt[i], i};
    }
    sort(F.begin(), F.end());
    
    int curr = 0;
    for (int i = 0; i < n; i++) {
        if (bwt[i] == '$') {
            curr = i;
            break;
        }
    }
    
    string original = "";
    for (int i = 0; i < n; i++) {
        original += F[curr].first;
        curr = F[curr].second;
    }
    return original;
}

void radixSort(int k) {
	int m = max(300, n+2);
	memset(&freqs[0], 0, sizeof(freqs));

	// freq count
	for (int i = 0; i < n; i++) {
		if (i + k < n)
			freqs[rankarray[i+k] + 1]++;
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
			tmpsuffarray[freqs[rankarray[suffarray[i] + k]+1]++] = suffarray[i];
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
	    s += "$";
		n = s.size();

		buildSuffArray();
		string bwt = "";
		
		for (int j = 0; j < n; j++) {
		    int currsuffix = suffarray[j];
		    if (currsuffix == 0) {
		        bwt += s[n - 1];
		    } else {
		        bwt += s[currsuffix - 1];
		    }
		}
		cout << bwt << endl;
		cout << inverseBWT(bwt) << endl;
	}

	return 0;
}
