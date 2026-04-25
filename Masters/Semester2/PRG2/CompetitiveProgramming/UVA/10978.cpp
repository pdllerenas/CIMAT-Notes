#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  while (cin >> n && n != 0)
  {
    vector<string> table(n, "");
    int current_pos = 0;

    for (int i = 0; i < n; ++i)
    {
      string card, word;
      cin >> card >> word;

      int len = word.length();
      int count = 0;

      while (true)
      {
        if (table[current_pos] == "")
        {
          count++;
          if (count == len)
            break;
        }
        current_pos = (current_pos + 1) % n;
      }

      table[current_pos] = card;

      current_pos = (current_pos + 1) % n;
    }

    for (int i = 0; i < n; ++i)
    {
      cout << table[i] << (i == n - 1 ? "" : " ");
    }
    cout << "\n";
  }
  return 0;
}