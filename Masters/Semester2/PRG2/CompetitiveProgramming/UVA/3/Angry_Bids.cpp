#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

void solve()
{
  int P, C;
  cin >> P >> C;

  vector<int> producers(P);
  vector<int> consumers(C);
  vector<int> candidates;

  // price 0 when no producers or consumers
  candidates.push_back(0);

  for (int i = 0; i < P; i++)
  {
    cin >> producers[i];
    candidates.push_back(producers[i]);
  }

  for (int i = 0; i < C; i++)
  {
    cin >> consumers[i];
    candidates.push_back(consumers[i]);
  }

  sort(producers.begin(), producers.end());
  sort(consumers.begin(), consumers.end());

  sort(candidates.begin(), candidates.end());
  // remove duplicates so we dont check a same value twice
  candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

  int min_angry = INT_MAX;
  int best_price = 0;

  for (int price : candidates)
  {
    // find first element strictly greater than price
    auto it_p = upper_bound(producers.begin(), producers.end(), price);

    // number of angry producers (ones with a bid greater than the candidate price)
    int angry_p = producers.end() - it_p;

    // find first element >= price
    auto it_c = lower_bound(consumers.begin(), consumers.end(), price);

    // number of angry consumers (ones with big lower than price)
    int angry_c = it_c - consumers.begin();

    int total_angry = angry_c + angry_p;

    if (total_angry < min_angry)
    {
      min_angry = total_angry;
      best_price = price;
    }
  }

  cout << best_price << " " << min_angry << '\n';
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;

  while (t--)
  {
    solve();
  }

  return 0;
}