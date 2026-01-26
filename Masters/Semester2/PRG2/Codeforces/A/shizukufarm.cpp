#include <cmath>
#include <iostream>

using namespace std;

// we want to check the number of
// configurations for the number
// of legs. Count chicken first, then
// group chicken legs to consider
// cows.
int solution(int legs) {
  // if odd, no possible
  // arrangement
  if (legs & 1) {
    return 0;
  }
  int c = 1;
  c += std::floor(legs / 4);
  return c;
}

int main() {
	int t;
	cin >> t;
	while (t--) {
		int n;
		cin >> n;
		cout << solution(n) << endl;
	}
}
