#include <array>
#include <iostream>
#include <unordered_set>

int main() {
  std::unordered_set<int> s;
  std::array<int, 10> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (int ai : a) {
    if (s.count(ai)) {
      std::cout << "repeated" << std::endl;
			return 0;
    } else {
      s.insert(ai);
    }
  }
	std::cout << "no repetitions" << std::endl;
}
