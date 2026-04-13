#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

void save_paraview(int Nx, int Ny, const vector<double> &x,
                   const vector<double> &y, double t,
                   const vector<vector<double>> &U, int nrec,
                   const string &folder = "output") {
  filesystem::create_directories(folder);

  stringstream ss;
  ss << folder << '/' << "S-" << setw(4) << setfill('0') << nrec << ".vtk";
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
}
