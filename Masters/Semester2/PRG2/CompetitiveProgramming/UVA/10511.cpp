#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <climits>
#include <unordered_map>
#include <unordered_set>
using namespace std;

typedef struct Person
{
  string name;
  string party;
  vector<string> clubs;
} Person;

typedef struct Representative
{
  string name;
  string club;
} Representative;

class Graph
{
private:
  int _size;
  vector<vector<int>> _adj;
  vector<vector<int>> _neighbors;

public:
  Graph(int size) : _size(size)
  {
    _adj.resize(size, vector<int>(size, 0));
    _neighbors.resize(size);
  }

  bool has_flow(int u, int v)
  {
    return _adj[v][u] > 0;
  }

  void add_edge(int u, int v, int c)
  {
    _adj[u][v] = c;
    _neighbors[u].push_back(v);
    _neighbors[v].push_back(u);
  }

  int bfs(int s, int t, vector<int> &parent)
  {
    vector<bool> visited(_size, false);
    queue<int> q;
    q.push(s);

    visited[s] = true;

    while (!q.empty())
    {
      int u = q.front();
      q.pop();
      if (u == t)
        break;
      for (int v : _neighbors[u])
      {
        int val = _adj[u][v];
        if (!visited[v] && val > 0)
        {
          q.push(v);
          visited[v] = true;
          parent[v] = u;
        }
      }
    }
    return visited[t];
  }

  int edmonds_karp(int source, int sink)
  {
    vector<int> parent(_size, -1);
    int max_flow = 0;

    while (bfs(source, sink, parent))
    {
      int path_flow = INT_MAX;
      int s = sink;

      while (s != source)
      {
        path_flow = min(path_flow, _adj[parent[s]][s]);
        s = parent[s];
      }

      max_flow += path_flow;
      int v = sink;

      while (v != source)
      {
        int u = parent[v];
        _adj[u][v] -= path_flow;
        _adj[v][u] += path_flow;
        v = parent[v];
      }
    }
    return max_flow;
  }
};

void solve(const vector<Person> &people)
{
  unordered_set<string> clubs;
  unordered_set<string> parties;

  for (const Person &p : people)
  {
    parties.insert(p.party);
    for (const string &c : p.clubs)
    {
      clubs.insert(c);
    }
  }

  int num_parties = parties.size();
  int num_people = people.size();
  int num_clubs = clubs.size();

  // 2 more for source and sink
  int total_nodes = 2 + num_clubs + num_parties + num_people;

  Graph g(total_nodes);
  int source = 0;
  int club_offset = 1;
  int people_offset = club_offset + num_clubs;
  int parties_offset = people_offset + num_people;
  int sink = total_nodes - 1;

  unordered_map<string, int> club_ids;
  unordered_map<string, int> party_ids;

  int curr_id = club_offset;
  for (const string &club : clubs)
  {
    club_ids[club] = curr_id;
    g.add_edge(source, curr_id, 1);
    curr_id++;
  }

  curr_id = parties_offset;
  int max_party_members = (num_clubs - 1) / 2;
  for (const string &party : parties)
  {
    party_ids[party] = curr_id;
    g.add_edge(curr_id, sink, max_party_members);
    curr_id++;
  }

  for (int i = 0; i < num_people; i++)
  {
    int p_id = people_offset + i;
    g.add_edge(p_id, party_ids[people[i].party], 1);

    for (const string &club : people[i].clubs)
    {
      g.add_edge(club_ids[club], p_id, 1);
    }
  }

  int max_flow = g.edmonds_karp(source, sink);
  if (max_flow == num_clubs)
  {
    for (const string &club : clubs)
    {
      int club_id = club_ids[club];
      for (int i = 0; i < num_people; i++)
      {
        int person_id = people_offset + i;
        if (g.has_flow(club_id, person_id))
        {
          cout << people[i].name << ' ' << club << '\n';
        }
      }
    }
  }
  else
  {
    cout << "Impossible.\n";
  }
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  if (!getline(cin, line))
    return 0;

  int t = stoi(line);
  getline(cin, line);

  while (t--)
  {
    vector<Person> people;

    while (getline(cin, line))
    {
      if (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos)
      {
        break;
      }

      istringstream iss(line);
      Person data;
      string club;

      iss >> data.name >> data.party;

      while (iss >> club)
      {
        data.clubs.push_back(club);
      }

      people.push_back(data);
    }
    solve(people);
    if (t > 0)
    {
      cout << '\n';
    }
  }

  return 0;
}