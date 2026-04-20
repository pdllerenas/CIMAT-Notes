#include <iostream>
#include <vector>
#include <cmath>
#include <filesystem>
#include "vtk_reader.h"
#include "vtk_writer.h"
#include "triangle_mesh.h"

double f(double x, double y)
{
  return 20 * (std::sin(4 * x) * std::sin(3 * y) + 0.3 * std::cos(6 * x) * std::sin(5 * y) + 1);
}

std::vector<std::array<double, 2>> getGridCellGradients(const TriangleMesh &mesh)
{
  int numTriangles = mesh.getNumTriangles();
  const auto &centroids = mesh.getCentroids();
  const auto &cellNeighbors = mesh.getCellNeighbors();
  const auto &weightX = mesh.getWeightX();
  const auto &weightY = mesh.getWeightY();

  std::vector<std::array<double, 2>> cellGradients(numTriangles);

  for (int cellId = 0; cellId < numTriangles; ++cellId)
  {
    auto centroid = centroids[cellId];
    auto neighbors = cellNeighbors[cellId];

    double gradX = 0.0;
    double gradY = 0.0;

    for (size_t i = 0; i < neighbors.size(); ++i)
    {
      double Sf = f(centroids[neighbors[i]][0], centroids[neighbors[i]][1]) -
                  f(centroid[0], centroid[1]);

      gradX += weightX[cellId][i] * Sf;
      gradY += weightY[cellId][i] * Sf;
    }

    cellGradients[cellId] = {gradX, gradY};
  }

  return cellGradients;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <filename.vtk> <output_name>\n";
    return 1;
  }

  vtkSmartPointer<vtkUnstructuredGrid> grid = readVTKFile(argv[1]);
  if (!grid)
  {
    std::cerr << "Failed to load grid.\n";
    return 1;
  }

  TriangleMesh mesh(grid);

  auto start_time = std::chrono::high_resolution_clock::now();

  std::vector<std::array<double, 2>> cellGradients = getGridCellGradients(mesh);

  std::vector<std::array<double, 2>> nodeGradients(mesh.getNumNodes(), {0.0, 0.0});
  const auto &nodeMap = mesh.getNodeToCells();

  // node interpolation
  for (int nodeId = 0; nodeId < mesh.getNumNodes(); ++nodeId)
  {
    const auto &cells = nodeMap[nodeId];
    if (cells.empty())
      continue;

    double numConnected = static_cast<double>(cells.size());
    double sumX = 0.0;
    double sumY = 0.0;

    for (int cellId : cells)
    {
      sumX += cellGradients[cellId][0];
      sumY += cellGradients[cellId][1];
    }

    nodeGradients[nodeId][0] = sumX / numConnected;
    nodeGradients[nodeId][1] = sumY / numConnected;
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end_time - start_time;
  std::cout << "Sequential time: " << elapsed.count() << "s.\n";

  // export to vtk
  std::string outputFolder = "../output";
  if (!std::filesystem::exists(outputFolder))
  {
    std::filesystem::create_directory(outputFolder);
    std::cout << "Created directory: " << outputFolder << "\n";
  }
  std::string fullPath = outputFolder + "/" + argv[2] + ".vtk";
  exportToParaView(fullPath, mesh, nodeGradients);

  return 0;
}