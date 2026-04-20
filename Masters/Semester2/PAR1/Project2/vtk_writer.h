#pragma once

#include <string>
#include <vector>
#include "triangle_mesh.h"

void exportToParaView(const std::string& filename, const TriangleMesh& mesh, const std::vector<std::array<double, 2>>& nodeGradients);