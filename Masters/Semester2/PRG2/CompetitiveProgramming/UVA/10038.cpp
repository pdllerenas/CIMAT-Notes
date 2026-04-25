#include <iostream>
#include <set>

using namespace std;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  while (cin >> n)
  {
    set<int> s;
    int prev, curr;
    cin >> prev;
    for (int i = 1; i < n; i++)
    {
      cin >> curr;
      int diff = abs(prev - curr);
      if (diff >= 1 && diff <= n-1)
      {
        s.insert(diff);
      }
      prev = curr;
    }
    if (s.size() == n - 1)
    {
      cout << "Jolly\n";
    }
    else
    {
      cout << "Not jolly\n";
    }
  }
}