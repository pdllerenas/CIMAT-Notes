#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
	// [+-]?\d+[\.\d+[eE][+-]?\d+]?
  const std::regex pattern("[+-]?\\d+(\\.\\d+[eE][+-]?\\d+)?");
	std::string s = "1.2e10";

  std::cout << s << ": " << std::regex_match(s, pattern)
            << std::endl;
}
