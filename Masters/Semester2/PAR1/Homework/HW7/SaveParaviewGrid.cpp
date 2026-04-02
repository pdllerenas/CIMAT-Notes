// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//------------------------------------------------------------
//    PROGRAMA: Solucion de la ecuacion de calor en Serie
//                Miguel Angel Uh Zapata
//                         2026
//------------------------------------------------------------
//  HOW TO RUN:
//  g++ -std=c++11 -O2 SaveParaviewGrid.cpp -o SaveParaviewGrid
//  ./SaveParaviewGrid
//------------------------------------------------------------
// ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

//------------------------------------------------------------
// Función para guardar en formato VTK (ParaView)
//------------------------------------------------------------
void save_paraview(int Nx, int Ny, const vector<double> &x,
                   const vector<double> &y, double t,
                   const vector<vector<double>> &U, int nrec) {
  stringstream ss;
  ss << "S-" << setw(4) << setfill('0') << nrec << ".vtk";
  string filename = ss.str();

  ofstream file(filename);

  file << "# vtk DataFile Version 2.0\n";
  file << "Sample rectilinear grid\n";
  file << "ASCII\n";
  file << "DATASET RECTILINEAR_GRID\n";
  file << "DIMENSIONS " << Nx << " " << Ny << " 1\n";

  // Coordenadas
  file << "X_COORDINATES " << Nx << " float\n";
  for (int i = 0; i < Nx; i++)
    file << scientific << setprecision(5) << x[i] << "\n";

  file << "Y_COORDINATES " << Ny << " float\n";
  for (int j = 0; j < Ny; j++)
    file << scientific << setprecision(5) << y[j] << "\n";

  file << "Z_COORDINATES 1 float\n";
  file << "0.0\n";

  // Datos
  file << "POINT_DATA " << Nx * Ny << "\n";
  file << "SCALARS U float\n";
  file << "LOOKUP_TABLE default\n";

  for (int j = 0; j < Ny; j++)
    for (int i = 0; i < Nx; i++)
      file << scientific << setprecision(5) << U[i][j] << "\n";

  file.close();

  cout << "\n    Paraview No. " << nrec << ", time: " << t << endl;
}

//------------------------------------------------------------
// Programa principal
//------------------------------------------------------------
int main() {
  int Nx = 50, Ny = 50, Nt = 10000;
  int i, j, n;
  const double pi = M_PI;

  double xI = 0.0, xF = 2.0;
  double yI = 0.0, yF = 2.0;
  double tI = 0.0, tF = 1.0;

  double Dx = (xF - xI) / (Nx - 1);
  double Dy = (yF - yI) / (Ny - 1);
  double Dt = (tF - tI) / (Nt - 1);

  //------------------------------------------------------------
  // Mallas
  //------------------------------------------------------------
  vector<double> x(Nx), y(Ny);

  for (i = 0; i < Nx; i++)
    x[i] = xI + i * Dx;

  for (j = 0; j < Ny; j++)
    y[j] = yI + j * Dy;

  //------------------------------------------------------------
  // Solución
  //------------------------------------------------------------
  vector<vector<double>> Unew(Nx, vector<double>(Ny, 0.0));

  int nrec = 0;
  double t = 0.0;

  //------------------------------------------------------------
  // Iteraciones
  //------------------------------------------------------------
  for (n = 1; n <= Nt; n++) {

    t = tI + n * Dt;

    // Valores de la funcion
    for (i = 0; i < Nx; i++) {
      for (j = 0; j < Ny; j++) {
        Unew[i][j] = exp(-t) * sin(pi * x[i]) * sin(pi * y[j]);
      }
    }

    // Guardar
    if (n % 100 == 0) {
      nrec++;
      save_paraview(Nx, Ny, x, y, t, Unew, nrec);
    }
  }

  return 0;
}
