#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<string> table(n, "");
  for (int i = 0; i < n; ++i)
  {
    string name;
    cin >> name;
    table[i] = name;
  }

  int q;
  cin >> q;

  for (int i = 0; i < q; ++i)
  {
    string name;
    cin >> name;
    table[i] = name;
  }

  return 0;
}