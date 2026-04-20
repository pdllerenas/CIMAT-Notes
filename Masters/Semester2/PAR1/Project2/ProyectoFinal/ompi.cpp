#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <map>
#include <set>
#include "vtk_reader.h"
#include "vtk_writer.h"
#include "triangle_mesh.h"

double f(double x, double y)
{
    return 20 * (std::sin(4 * x) * std::sin(3 * y) + 0.3 * std::cos(6 * x) * std::sin(5 * y) + 1);
}

int getOwner(int cellId, int totalTriangles, int numProcs)
{
    int baseChunk = totalTriangles / numProcs;
    int remainder = totalTriangles % numProcs;

    int cutoff = remainder * (baseChunk + 1);

    if (cellId < cutoff)
    {
        return cellId / (baseChunk + 1);
    }
    else
    {
        return remainder + (cellId - cutoff) / baseChunk;
    }
}

std::vector<std::array<double, 2>> getGridPartitionGradients(
    const TriangleMesh &mesh,
    int rank, int numProcs, int totalTriangles,
    const std::map<int, std::set<int>> &sendMap,
    const std::map<int, std::set<int>> &recvMap)
{
    int numNodes = mesh.getNumNodes();
    const auto &centroids = mesh.getCentroids();
    const auto &cellNeighbors = mesh.getCellNeighbors();
    const auto &weightX = mesh.getWeightX();
    const auto &weightY = mesh.getWeightY();
    const auto &nodeMap = mesh.getNodeToCells();

    int myStart = 0;
    for (int i = 0; i < rank; ++i)
    {
        myStart += (totalTriangles / numProcs) + (i < (totalTriangles % numProcs) ? 1 : 0);
    }
    int localNumTriangles = (totalTriangles / numProcs) + (rank < (totalTriangles % numProcs) ? 1 : 0);
    int myEnd = myStart + localNumTriangles;

    std::vector<double> sendBuffer;
    std::vector<double> recvBuffer;
    std::map<int, int> sendOffsets;
    std::map<int, int> recvOffsets;

    for (const auto &[targetRank, cellsToSend] : sendMap)
    {
        sendOffsets[targetRank] = sendBuffer.size();
        for (int cellId : cellsToSend)
        {
            sendBuffer.push_back(f(centroids[cellId][0], centroids[cellId][1]));
        }
    }

    for (const auto &[sourceRank, cellsToRecv] : recvMap)
    {
        recvOffsets[sourceRank] = recvBuffer.size();
        recvBuffer.resize(recvBuffer.size() + cellsToRecv.size());
    }

    std::vector<MPI_Request> requests;

    for (const auto &[sourceRank, cellsToRecv] : recvMap)
    {
        MPI_Request req;
        MPI_Irecv(&recvBuffer[recvOffsets[sourceRank]], cellsToRecv.size(),
                  MPI_DOUBLE, sourceRank, 0, MPI_COMM_WORLD, &req);
        requests.push_back(req);
    }

    for (const auto &[targetRank, cellsToSend] : sendMap)
    {
        MPI_Request req;
        MPI_Isend(&sendBuffer[sendOffsets[targetRank]], cellsToSend.size(),
                  MPI_DOUBLE, targetRank, 0, MPI_COMM_WORLD, &req);
        requests.push_back(req);
    }

    MPI_Waitall(requests.size(), requests.data(), MPI_STATUSES_IGNORE);

    std::map<int, double> ghostValues;
    for (const auto &[sourceRank, cellsToRecv] : recvMap)
    {
        int offset = recvOffsets[sourceRank];
        int i = 0;
        for (int globalCellId : cellsToRecv)
        {
            ghostValues[globalCellId] = recvBuffer[offset + i++];
        }
    }

    std::vector<std::array<double, 2>> localCellGradients(localNumTriangles, {0.0, 0.0});

    for (int cellId = myStart; cellId < myEnd; ++cellId)
    {
        auto centroid = centroids[cellId];
        auto neighbors = cellNeighbors[cellId];
        double currentTemp = f(centroid[0], centroid[1]);

        double gradX = 0.0;
        double gradY = 0.0;

        for (size_t i = 0; i < neighbors.size(); ++i)
        {
            int neighborId = neighbors[i];
            int neighborOwner = getOwner(neighborId, totalTriangles, numProcs);

            double neighborTemp;

            if (neighborOwner == rank)
            {
                neighborTemp = f(centroids[neighborId][0], centroids[neighborId][1]);
            }
            else
            {
                neighborTemp = ghostValues[neighborId];
            }

            double deltaT = neighborTemp - currentTemp;
            gradX += weightX[cellId][i] * deltaT;
            gradY += weightY[cellId][i] * deltaT;
        }

        int localIdx = cellId - myStart;
        localCellGradients[localIdx] = {gradX, gradY};
    }

    return localCellGradients;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, numProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    if (argc != 3)
    {
        if (rank == 0)
            std::cerr << "Usage: mpiexec -n <procs> " << argv[0] << " <filename.vtk> <output_name>\n";
        MPI_Finalize();
        return 1;
    }

    vtkSmartPointer<vtkUnstructuredGrid> grid = readVTKFile(argv[1]);
    if (!grid)
    {
        MPI_Finalize();
        return 1;
    }
    TriangleMesh mesh(grid);
    int totalTriangles = mesh.getNumTriangles();

    // determine my local partition range (column partitioning)
    int myStart = 0;
    for (int i = 0; i < rank; ++i)
    {
        myStart += (totalTriangles / numProcs) + (i < (totalTriangles % numProcs) ? 1 : 0);
    }
    int localNumTriangles = (totalTriangles / numProcs) + (rank < (totalTriangles % numProcs) ? 1 : 0);
    int myEnd = myStart + localNumTriangles;

    std::map<int, std::set<int>> sendMap;
    std::map<int, std::set<int>> recvMap;
    const auto &cellNeighbors = mesh.getCellNeighbors();

    for (int cellId = myStart; cellId < myEnd; ++cellId)
    {
        for (int neighborId : cellNeighbors[cellId])
        {
            int neighborOwner = getOwner(neighborId, totalTriangles, numProcs);
            if (neighborOwner != rank)
            {
                recvMap[neighborOwner].insert(neighborId);
                sendMap[neighborOwner].insert(cellId);
            }
        }
    }

    // sync all processes before computing gradient
    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();
    std::vector<std::array<double, 2>> localGradients =
        getGridPartitionGradients(mesh, rank, numProcs, totalTriangles, sendMap, recvMap);

    int numNodes = mesh.getNumNodes();

    std::vector<double> localSumX(numNodes, 0.0);
    std::vector<double> localSumY(numNodes, 0.0);
    std::vector<int> localCount(numNodes, 0);

    const auto &nodeMap = mesh.getNodeToCells();

    for (int nodeId = 0; nodeId < numNodes; ++nodeId)
    {
        const auto &cells = nodeMap[nodeId];
        for (int cellId : cells)
        {
            if (cellId >= myStart && cellId < myEnd)
            {
                int localIdx = cellId - myStart;
                localSumX[nodeId] += localGradients[localIdx][0];
                localSumY[nodeId] += localGradients[localIdx][1];
                localCount[nodeId] += 1;
            }
        }
    }

    std::vector<double> globalSumX;
    std::vector<double> globalSumY;
    std::vector<int> globalCount;

    if (rank == 0)
    {
        globalSumX.resize(numNodes, 0.0);
        globalSumY.resize(numNodes, 0.0);
        globalCount.resize(numNodes, 0);
    }

    // local sums for interpolation 
    MPI_Reduce(localSumX.data(), globalSumX.data(), numNodes, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(localSumY.data(), globalSumY.data(), numNodes, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(localCount.data(), globalCount.data(), numNodes, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::vector<std::array<double, 2>> globalNodeGradients(numNodes, {0.0, 0.0});

        for (int nodeId = 0; nodeId < numNodes; ++nodeId)
        {
            if (globalCount[nodeId] > 0)
            {
                globalNodeGradients[nodeId][0] = globalSumX[nodeId] / globalCount[nodeId];
                globalNodeGradients[nodeId][1] = globalSumY[nodeId] / globalCount[nodeId];
            }
        }

        double end_time = MPI_Wtime();
        std::cout << "Total Parallel Pipeline Time (" << numProcs << " procs): "
                  << (end_time - start_time) << " seconds\n";

        std::string fullPath = std::string("../output/") + argv[2] + ".vtk";
        exportToParaView(fullPath, mesh, globalNodeGradients);
        std::cout << "Master Rank completed export successfully to " << fullPath << "\n";
    }

    MPI_Finalize();
    return 0;
}