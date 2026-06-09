#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <tuple>

using namespace std;

struct Pole
{
  int x, y, z;
  int rx, ry;  // simplified point, x / gcd(x,y) and y /  gcd(x,y)
  long long d; // distance^2 from origin: x^2 + y^2

  Pole() {}

  Pole(int _x, int _y, int _z) : x(_x), y(_y), z(_z)
  {
    int g = __gcd(abs(x), abs(y));
    rx = x / g;
    ry = y / g;
    d = 1LL * x * x + 1LL * y * y;
  }

  bool operator<(const Pole &other) const
  {
    return tie(rx, ry, d) < tie(other.rx, other.ry, other.d);
  }

  bool operator==(const Pole &other) const
  {
    return tie(rx, ry) == tie(other.rx, other.ry);
  }
};

void solve()
{
  int N;
  int test_case = 1;
  while (cin >> N && N != 0)
  {
    vector<Pole> poles(N);
    for (int i = 0; i < N; i++)
    {
      int x, y, z;
      cin >> x >> y >> z;
      poles[i] = Pole(x, y, z);
    }

    // sorted by rx then ry then d. Therefore, when we loop through it,
    // we can assume that the previous pole is closer to the center
    sort(poles.begin(), poles.end());
    int max_height_seen = poles[0].z;
    Pole prev_pole = poles[0];

    vector<Pole> hidden;

    for (int i = 1; i < N; i++)
    {
      // use overloaded == to check for rx and ry equality
      if (poles[i] == prev_pole)
      {
        // since previous pole is closer to origin, current pole light is hidden
        if (poles[i].z <= max_height_seen)
        {
          hidden.push_back(poles[i]);
        }
        else
        {
          max_height_seen = poles[i].z;
        }
      }
      else
      {
        max_height_seen = poles[i].z;
      }
      prev_pole = poles[i];
    }

    // sort by x and y
    sort(hidden.begin(), hidden.end(), [](const Pole &a, const Pole &b)
         { return tie(a.x, a.y) < tie(b.x, b.y); });

    cout << "Data set " << test_case << ":\n";
    if (hidden.empty())
    {
      cout << "All the lights are visible.\n";
    }
    else
    {
      cout << "Some lights are not visible:\n";
      for (size_t i = 0; i < hidden.size(); i++)
      {
        cout << "x = " << hidden[i].x << ", y = " << hidden[i].y << (i < hidden.size() - 1 ? ";" : ".") << '\n';
      }
    }
    test_case++;
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  solve();

  return 0;
}