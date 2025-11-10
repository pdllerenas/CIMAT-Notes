#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/mse.hpp"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using namespace interp::mse;
using namespace file_input;

double sutherland(double T, double m0 = 1.919e-2, double T0 = 273.0,
                  double Su = 139.0) {
  return m0 * std::pow(1000 * T / T0, 1.5) * (T0 + Su) / (1000 * T + Su);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Invalid argument count. Usage: " << argv[0]
              << "<Ti.txt> <lambda.txt>\n";
  }
  std::vector<double> x = readFile<double>(argv[1]);
  std::vector<double> y;

  // ======== a ===========

  if (!std::filesystem::exists("ex/2/sutherland")) {
    std::filesystem::create_directories("ex/2/sutherland");
  }
  std::ofstream eval_table("ex/2/sutherland/eval.csv");

  eval_table << "x,y\n";
  for (double xi : x) {
    double s = sutherland(xi);
    y.push_back(s);
    eval_table << xi << ',' << s << '\n';
  }
  eval_table.close();

  // ======== a ===========

  // ======== b ===========

  std::vector<double> lambda = readFile<double>(argv[2]);
  std::vector<double> coeff;
  double a;
  if (!std::filesystem::exists("ex/2/inter")) {
    std::filesystem::create_directories("ex/2/inter");
  }
  std::ofstream poly_table("ex/2/inter/poly.csv");
  std::ofstream cos_table("ex/2/inter/cos.csv");
  std::ofstream rad_table("ex/2/inter/rad.csv");

  poly_table << "T,lambda,eval,err_abs\n";
  cos_table << "T,lambda,eval,err_abs\n";
  rad_table << "T,lambda,eval,err_abs\n";

  double s = sutherland(1.2);
  for (double l : lambda) {
    coeff = polynomial(x, y, l, 5);
    a = eval_basis(1.2, coeff, phi_poly);
    poly_table << 1.2 << ',' << l << ',' << a << ',' << std::abs(s - a)
               << std::endl;

    coeff = cosine(x, y, l, 5);
    a = eval_basis(1.2, coeff, phi_trig);
    cos_table << 1.2 << ',' << l << ',' << a << ',' << std::abs(s - a)
              << std::endl;

    coeff = radial(x, y, l);
    a = eval_basis_rad(1.2, coeff, x);
    rad_table << 1.2 << ',' << l << ',' << a << ',' << std::abs(s - a)
              << std::endl;
  }

  // ======== b ===========

  // ======== c ===========

  // ======== c ===========
}
