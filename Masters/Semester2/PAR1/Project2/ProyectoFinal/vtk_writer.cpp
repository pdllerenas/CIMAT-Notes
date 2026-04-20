#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "vtk_writer.h"

using namespace std;

void exportToParaView(const std::string& filename, const TriangleMesh& mesh, const std::vector<std::array<double, 2>>& nodeGradients)
{
    std::ofstream file(filename);

    int numNodes = mesh.getNumNodes();
    int numCells = mesh.getNumTriangles();
    const auto& nodes = mesh.getNodes();
    const auto& connectivity = mesh.getConnectivity();
    const auto& cellOffsets = mesh.getCellOffsets();

    file << "# vtk DataFile Version 2.0\n";
    file << "Mesh Calculus Output\n";
    file << "ASCII\n";
    file << "DATASET UNSTRUCTURED_GRID\n";

    // 1. Write the Points (Nodes)
    file << "POINTS " << numNodes << " double\n";
    for (int i = 0; i < numNodes; i++) {
        file << nodes[i][0] << " " << nodes[i][1] << " " << nodes[i][2] << "\n";
    }

    // 2. Write the Connectivity (Cells)
    // ParaView needs to know how many integers to read. For triangles, it's 4 integers per cell:
    // [Number of points in cell] [NodeA] [NodeB] [NodeC] -> e.g., "3 0 1 5"
    int total_size = numCells * 4; 
    file << "CELLS " << numCells << " " << total_size << "\n";
    for (int i = 0; i < numCells; i++) {
        int start = cellOffsets[i];
        file << "3 " << connectivity[start] << " " << connectivity[start+1] << " " << connectivity[start+2] << "\n";
    }

    // 3. Write Cell Types (5 = VTK_TRIANGLE)
    file << "CELL_TYPES " << numCells << "\n";
    for (int i = 0; i < numCells; i++) {
        file << "5\n";
    }

    // 4. Write the Gradient Data
    file << "POINT_DATA " << numNodes << "\n";
    
    // By declaring this as "VECTORS", ParaView automatically groups the components 
    // together so you can apply the "Glyph" filter to see arrows!
    file << "VECTORS Gradient double\n";
    for (int i = 0; i < numNodes; i++) {
        file << std::scientific << std::setprecision(8) 
             << nodeGradients[i][0] << " " 
             << nodeGradients[i][1] << " 0.0\n"; // VTK vectors require a 3rd dimension (Z), so we pad with 0.0
    }

    file.close();
    std::cout << "Successfully exported data to " << filename << "\n";
}