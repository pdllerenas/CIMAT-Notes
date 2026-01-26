#include <iostream>

// if we can divide w into 2 even numbers.
// w = 2a + 2b = 2(a+b) = 2c
// thus, we check if it is even
bool solution(int w) {
	return ~w & 1;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return -1;
  }
  int w = atoi(argv[1]);
  if (solution(w)) {
    std::cout << "YES" << std::endl;
  } else {
    std::cout << "NO" << std::endl;
  }
}
