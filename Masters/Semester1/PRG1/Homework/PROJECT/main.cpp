#include <iostream>
#include <vector>
using namespace std;

int main() {
  const int H = 20, W = 40;
  vector<vector<int>> grid(H, vector<int>(W)), nextGrid(H, vector<int>(W));

  // Example: a glider
  grid[1][2] = grid[2][3] = grid[3][1] = grid[3][2] = grid[3][3] = 1;

  while (true) {
    system("clear"); // or "cls" on Windows

    // Print grid
    for (int y = 0; y < H; y++) {
      for (int x = 0; x < W; x++)
        cout << (grid[y][x] ? "█" : " ");
      cout << "\n";
    }

    // Compute next generation
    for (int y = 0; y < H; y++) {
      for (int x = 0; x < W; x++) {
        int neighbors = 0;
        for (int dy = -1; dy <= 1; dy++)
          for (int dx = -1; dx <= 1; dx++)
            if (!(dx == 0 && dy == 0)) {
              int ny = y + dy, nx = x + dx;
              if (ny >= 0 && ny < H && nx >= 0 && nx < W)
                neighbors += grid[ny][nx];
            }

        nextGrid[y][x] = (grid[y][x] && (neighbors == 2 || neighbors == 3)) ||
                         (!grid[y][x] && neighbors == 3);
      }
    }

    grid.swap(nextGrid);
    cin.get(); // step manually
  }
}
