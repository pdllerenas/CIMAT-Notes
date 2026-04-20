#include "triangle_mesh.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkCellType.h>

TriangleMesh::TriangleMesh(vtkSmartPointer<vtkUnstructuredGrid> grid) : numTriangles(0)
{
    if (!grid)
        return;

    extractNodes(grid);
    extractTriangles(grid);
    buildAdjacency();
    buildNodeMap();
    calculateCentroids();
    precomputeGradientWeights();
}

/**
 * @brief Extracts node coordinates from the VTK grid and stores them in
 * the nodes vector
 *
 * @param grid
 */
void TriangleMesh::extractNodes(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    vtkPoints *points = grid->GetPoints();
    vtkIdType numPoints = points->GetNumberOfPoints();
    nodes.reserve(numPoints);

    for (vtkIdType i = 0; i < numPoints; ++i)
    {
        double p[3];
        points->GetPoint(i, p);
        nodes.push_back({p[0], p[1], p[2]});
    }
}

/**
 * @brief Extracts triangle elements from the VTK grid and stores
 * their connectivity in the connectivity vector, while also populating
 * the cellOffsets vector to indicate where each triangle's connectivity starts
 *
 * @param grid The input VTK unstructured grid
 */
void TriangleMesh::extractTriangles(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    vtkIdType totalEntities = grid->GetNumberOfCells();
    vtkNew<vtkIdList> cellPointsId;
    cellOffsets.push_back(0);

    for (vtkIdType i = 0; i < totalEntities; ++i)
    {
        if (grid->GetCellType(i) == VTK_TRIANGLE)
        {
            grid->GetCellPoints(i, cellPointsId);
            vtkIdType numIds = cellPointsId->GetNumberOfIds();

            for (vtkIdType j = 0; j < numIds; ++j)
            {
                connectivity.push_back(cellPointsId->GetId(j));
            }
            cellOffsets.push_back(connectivity.size());
            numTriangles++;
        }
    }
}

/**
 * @brief Builds adjacency matrix for triangle neighbors based on shared edges.
 *
 */
void TriangleMesh::buildAdjacency()
{
    std::map<std::pair<vtkIdType, vtkIdType>, std::vector<int>> edgeToCells;

    for (int cellId = 0; cellId < numTriangles; ++cellId)
    {
        int start = cellOffsets[cellId];
        vtkIdType n0 = connectivity[start];
        vtkIdType n1 = connectivity[start + 1];
        vtkIdType n2 = connectivity[start + 2];

        std::pair<vtkIdType, vtkIdType> e1 = {std::min(n0, n1), std::max(n0, n1)};
        std::pair<vtkIdType, vtkIdType> e2 = {std::min(n1, n2), std::max(n1, n2)};
        std::pair<vtkIdType, vtkIdType> e3 = {std::min(n2, n0), std::max(n2, n0)};

        edgeToCells[e1].push_back(cellId);
        edgeToCells[e2].push_back(cellId);
        edgeToCells[e3].push_back(cellId);
    }

    cellNeighbors.resize(numTriangles);
    for (const auto &[edge, cells] : edgeToCells)
    {
        if (cells.size() == 2)
        {
            cellNeighbors[cells[0]].push_back(cells[1]);
            cellNeighbors[cells[1]].push_back(cells[0]);
        }
    }
}

/**
 * @brief Builds a mapping from nodes to the cells that contain them
 *
 */

void TriangleMesh::buildNodeMap()
{
    nodeToCells.resize(nodes.size());
    for (int cellId = 0; cellId < numTriangles; ++cellId)
    {
        int start = cellOffsets[cellId];
        int end = cellOffsets[cellId + 1];
        for (int j = start; j < end; ++j)
        {
            nodeToCells[connectivity[j]].push_back(cellId);
        }
    }
}

/**
 * @brief Calculates centroids for each triangle cell
 *
 */

void TriangleMesh::calculateCentroids()
{
    centroids.assign(numTriangles, {0.0, 0.0, 0.0});
    for (int cellId = 0; cellId < numTriangles; ++cellId)
    {
        int start = cellOffsets[cellId];
        vtkIdType n0 = connectivity[start];
        vtkIdType n1 = connectivity[start + 1];
        vtkIdType n2 = connectivity[start + 2];

        double cx = (nodes[n0][0] + nodes[n1][0] + nodes[n2][0]) / 3.0;
        double cy = (nodes[n0][1] + nodes[n1][1] + nodes[n2][1]) / 3.0;
        double cz = (nodes[n0][2] + nodes[n1][2] + nodes[n2][2]) / 3.0;

        centroids[cellId] = {cx, cy, cz};
    }
}

/**
 * @brief Precomputes geometric weights for gradient calculation based on centroids and neighbors
 *
 */

void TriangleMesh::precomputeGradientWeights()
{
    weightX.resize(numTriangles);
    weightY.resize(numTriangles);

    for (int cellId = 0; cellId < numTriangles; ++cellId)
    {
        auto centroid = centroids[cellId];
        auto neighbors = cellNeighbors[cellId];

        weightX[cellId].resize(neighbors.size(), 0.0);
        weightY[cellId].resize(neighbors.size(), 0.0);

        double Sxx = 0.0, Syy = 0.0, Sxy = 0.0;

        for (int neighbor : neighbors)
        {
            double Sx = centroids[neighbor][0] - centroid[0];
            double Sy = centroids[neighbor][1] - centroid[1];
            Sxx += Sx * Sx;
            Syy += Sy * Sy;
            Sxy += Sx * Sy;
        }

        double D = Sxx * Syy - Sxy * Sxy;

        if (std::abs(D) > 1e-12)
        {
            for (size_t i = 0; i < neighbors.size(); ++i)
            {
                double Sx = centroids[neighbors[i]][0] - centroid[0];
                double Sy = centroids[neighbors[i]][1] - centroid[1];

                weightX[cellId][i] = (Syy * Sx - Sxy * Sy) / D;
                weightY[cellId][i] = (Sxx * Sy - Sxy * Sx) / D;
            }
        }
    }
}