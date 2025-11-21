#include "integer.hpp"
#include <iostream>

int main() {
  while (true) {
    std::string a, b;
    char op;
    std::cout << "\n\ninsert an operation '<a> <+|-> <b>'\n" << std::endl;
    std::cin >> a >> op >> b;
    UniversalInteger uni_a(a), uni_b(b);
    UniversalInteger uni_c;

    switch (op) {
    case '+': {
      uni_c = uni_a + uni_b;
      break;
    }
    case '-': {
      uni_c = uni_a - uni_b;
      break;
    }
    default:
      std::cerr << "invalid operation" << std::endl;
      continue;
    }
    std::cout << "int a: " << uni_a.get_int() << " " << op << std::endl
              << "int b: " << uni_b.get_int() << std::endl
              << "=================================================="
              << std::endl
              << "int c: " << uni_c.get_int() << '\n'
              << std::endl;
    std::cout << "bin a: " << uni_a.get_bin() << " " << op << std::endl
              << "bin b: " << uni_b.get_bin() << std::endl
              << "=================================================="
              << std::endl
              << "bin c: " << uni_c.get_bin() << '\n'
              << std::endl;
    std::cout << "hex a: " << uni_a.get_hex() << " " << op << std::endl
              << "hex b: " << uni_b.get_hex() << std::endl
              << "=================================================="
              << std::endl
              << "hex c: " << uni_c.get_hex() << std::endl;
  }
}
