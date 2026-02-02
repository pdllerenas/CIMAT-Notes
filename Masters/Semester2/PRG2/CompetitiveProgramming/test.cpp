#include <chrono>
#include <iostream>

using namespace std::chrono;

int main() {
  sys_days date = year{2010} / 8 / 9;

  weekday wd{date};

  std::cout << wd.c_encoding() << '\n';
}
