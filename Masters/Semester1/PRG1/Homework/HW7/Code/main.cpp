#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  const std::regex pattern(R"([+-]?\d+(\.\d+)?([eE][+-]?\d+)?)");
  std::string s;
  while (true) {
    std::cout << "Enter a number in scientific notation:" << std::endl;
    std::getline(std::cin, s);
    std::cout << ((std::regex_match(s, pattern) == 0) ? "Invalid.\n" : "Valid.\n")
              << std::endl;
  }
}
