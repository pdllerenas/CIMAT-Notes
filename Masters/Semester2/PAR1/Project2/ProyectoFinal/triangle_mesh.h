#pragma once

#include <vector>
#include <array>
#include <map>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

class TriangleMesh
{
public:
    TriangleMesh(vtkSmartPointer<vtkUnstructuredGrid> grid);

    int getNumNodes() const { return nodes.size(); }
    int getNumTriangles() const { return numTriangles; }

    const std::vector<std::array<double, 3>> &getNodes() const { return nodes; }
    const std::vector<vtkIdType> &getConnectivity() const { return connectivity; }
    const std::vector<int> &getCellOffsets() const { return cellOffsets; }
    const std::vector<std::array<double, 3>> &getCentroids() const { return centroids; }
    const std::vector<std::vector<int>> &getCellNeighbors() const { return cellNeighbors; }
    const std::vector<std::vector<int>> &getNodeToCells() const { return nodeToCells; }

    const std::vector<std::vector<double>> &getWeightX() const { return weightX; }
    const std::vector<std::vector<double>> &getWeightY() const { return weightY; }

private:
    int numTriangles;

    std::vector<std::array<double, 3>> nodes;
    std::vector<vtkIdType> connectivity;
    std::vector<int> cellOffsets;

    // triangle centroids and adjacency information
    std::vector<std::array<double, 3>> centroids;
    std::vector<std::vector<int>> cellNeighbors;
    std::vector<std::vector<int>> nodeToCells;

    // geometric gradient weights
    std::vector<std::vector<double>> weightX;
    std::vector<std::vector<double>> weightY;

    void extractNodes(vtkSmartPointer<vtkUnstructuredGrid> grid);
    void extractTriangles(vtkSmartPointer<vtkUnstructuredGrid> grid);
    void buildAdjacency();
    void buildNodeMap();
    void calculateCentroids();
    void precomputeGradientWeights();
};