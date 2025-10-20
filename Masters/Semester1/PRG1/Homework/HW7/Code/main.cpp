#include <iostream>
#include <regex>
#include <string>

char q0(char c) {
  if (c == '+' || c == '-' || (c <= '9' && c >= '0')) {
    return c;
  }
  return '\0';
}

char q1(char c) {
  if (c <= '9' && c >= '0') {
    return c;
  }
  return '\0';
}

char q2(char c) {
  if ((c <= '9' && c >= '0') || c == '.') {
    return c;
  }
  return '\0';
}

void fsm(std::string &s) {
  // copy s to corrected
  std::string corrected = s;

  size_t mantissa_size = 0;
  // traverse string s char by char
  size_t pos = 0;

  while (pos < s.size()) {
    char c = s[pos];
    char c0 = q0(c);
    if (c0 <= '9' || c0 >= '0') {
      c = s[++pos];
      while (q1(c) > 0) {
      }
    }
  }
  // if q0 approves of c
}

int main(int argc, char *argv[]) {
  const std::regex pattern(R"([+-]?\d+(\.\d+)?([eE][+-]?\d+)?)");
  std::string s;
  while (true) {
    std::cout << "Enter a number in scientific notation:" << std::endl;
    std::getline(std::cin, s);
    std::cout << ((std::regex_match(s, pattern) == 0) ? "Invalid.\n"
                                                      : "Valid.\n")
              << std::endl;
  }
}
