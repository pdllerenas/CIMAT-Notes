#include <stack>
#include <iostream>
#include <string>

using namespace std;


void solve()
{
  int queries;
  cin >> queries;
  stack<string> dream;
  string query;
  while (queries--)
  {
    cin >> query;
    if (query == "Test")
    {
      if (dream.empty())
      {
        cout << "Not in a dream\n";
      }
      else
      {
        cout << dream.top() << '\n';
      }
    }
    else if (query == "Kick")
    {
      if (!dream.empty())
      {
        dream.pop();
      }
    }
    else
    {
      string name;
      cin >> name;
      dream.push(name);
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