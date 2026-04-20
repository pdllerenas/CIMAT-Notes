cmake_minimum_required(VERSION 3.15)
project(MeshCalculus C CXX)

# Require C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O3)

find_package(VTK REQUIRED)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_CXX_INCLUDE_PATH})

add_executable(MeshCalculusOpenMPI
    ompi.cpp 
    vtk_reader.cpp
    vtk_writer.cpp
    triangle_mesh.cpp
)

target_link_libraries(MeshCalculusOpenMPI PRIVATE 
    ${MPI_CXX_LIBRARIES} 
    ${VTK_LIBRARIES}
)

vtk_module_autoinit(
  TARGETS MeshCalculusOpenMPI
  MODULES ${VTK_LIBRARIES}
)

add_executable(MeshCalculusSequential
    main.cpp 
    vtk_reader.cpp
    vtk_writer.cpp
    triangle_mesh.cpp
)

target_link_libraries(MeshCalculusSequential PRIVATE 
    ${VTK_LIBRARIES}
)

vtk_module_autoinit(
  TARGETS MeshCalculusSequential
  MODULES ${VTK_LIBRARIES}
)