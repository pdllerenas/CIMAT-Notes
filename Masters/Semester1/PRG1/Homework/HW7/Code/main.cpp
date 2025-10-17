#include <iostream>
#include <regex>

int main(int argc, char *argv[]) {
  const std::regex pattern("[\\+\\-]?\\d\\d+[.\\d\\d+[eE][\\+\\-]?\\d\\d+]?");
  std::smatch p_match;

  std::cout << "1.2e10" << ": " << std::regex_match("1.2e10", pattern)
            << std::endl;
}
