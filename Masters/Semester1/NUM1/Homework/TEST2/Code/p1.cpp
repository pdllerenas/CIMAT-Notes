#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/integration/gaussian.hpp"
#include "../../cpp-utils/numeric/integration/newton-cotes.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace file_input;
using namespace integration::newtoncotes;
using namespace integration::gaussian;

double f(double x) { return 1 / x; }

int main(int argc, char *argv[]) {
  if (!std::filesystem::exists("ex/p1")) {
    std::filesystem::create_directories("ex/p1");
  }
	constexpr double a = 1;
  constexpr double b = 3;

	const double ln3 = std::log(3); 

  std::ofstream outFile("ex/p1/closed.csv");

  outFile << "n,value,abserr\n";

  double A0 = closed::trapezoid(a, b, [](double x) { return f(x); });
  outFile << "1," << A0 << ',' << std::fabs(A0 - ln3) << '\n';
  double A1 = closed::simpson(a, b, [](double x) { return f(x); });
  outFile << "2," << A1 << ',' << std::fabs(A1 - ln3) << '\n';
  double A2 = closed::simpson38(a, b, [](double x) { return f(x); });
  outFile << "3," << A2 << ',' << std::fabs(A2 - ln3) << '\n';
  double A3 =
      closed::five_point(a, b, [](double x) { return f(x); });
  outFile << "4," << A3 << ',' << std::fabs(A3 - ln3) << '\n';

  std::ofstream outFile2("ex/p1/open.csv");

  outFile2 << "n,value,abserr\n";

  A0 = open::midpoint(a, b, [](double x) { return f(x); });
  outFile2 << "0," << A0 << ',' << std::fabs(A0 - ln3) << '\n';
  A1 = open::n2(a, b, [](double x) { return f(x); });
  outFile2 << "1," << A1 << ',' << std::fabs(A1 - ln3) << '\n';
  A2 = open::simpson38(a, b, [](double x) { return f(x); });
  outFile2 << "2," << A2 << ',' << std::fabs(A2 - ln3) << '\n';
  A3 = open::five_point(a, b, [](double x) { return f(x); });
  outFile2 << "3," << A3 << ',' << std::fabs(A3 - ln3) << '\n';

  std::ofstream outFile3("ex/p1/quadrature.csv");

  outFile3 << "n,value,abserr\n";

  A0 = quadrature(a, b, [](double x) { return f(x); }, 2);
  outFile3 << "2," << A0 << ',' << std::fabs(A0 - ln3) << '\n';
  A1 = quadrature(a, b, [](double x) { return f(x); }, 3);
  outFile3 << "3," << A1 << ',' << std::fabs(A1 - ln3) << '\n';
  A2 = quadrature(a, b, [](double x) { return f(x); }, 4);
  outFile3 << "4," << A2 << ',' << std::fabs(A2 - ln3) << '\n';
  A3 = quadrature(a, b, [](double x) { return f(x); }, 5);
  outFile3 << "5," << A3 << ',' << std::fabs(A3 - ln3) << '\n';
}
