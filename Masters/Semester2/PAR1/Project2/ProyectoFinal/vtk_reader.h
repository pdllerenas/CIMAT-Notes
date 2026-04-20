#pragma once

#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>

vtkSmartPointer<vtkUnstructuredGrid> readVTKFile(const char* filename);