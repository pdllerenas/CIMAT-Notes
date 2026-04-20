#include "vtk_reader.h"

vtkSmartPointer<vtkUnstructuredGrid> readVTKFile(const char* filename) {
  auto reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
  reader->SetFileName(filename);
  reader->Update();

  return reader->GetOutput();
}