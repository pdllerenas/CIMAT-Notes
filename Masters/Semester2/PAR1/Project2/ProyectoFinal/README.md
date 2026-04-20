## Dependencies
To compile and run this project, your system must have the following installed:
* **C++17** compatible compiler (GCC/Clang/MSVC)
* **CMake** (3.15 or higher)
* **VTK** (Visualization Toolkit)
* **MPI** (OpenMPI for Linux/macOS, or MS-MPI for Windows/MSYS2)
* **Gmsh** (Optional, required only to regenerate the `.vtk` meshes from the `.geo` script)

## Build Instructions

1. Open your terminal in the root directory of the project.
2. Create a build directory and navigate into it:
   `mkdir build && cd build`
3. Generate the Makefiles using CMake:
   `cmake --build . -j4`
4. Compile the executables:
   `make`

## Running the Code

Ensure you have an input `.vtk` mesh ready. You can generate one using the provided `.geo` script via Gmsh:
`gmsh manta_ray.geo -2 -format vtk -o input_mesh.vtk`

**Run the Sequential Version:**
`./MeshCalculusSequential path/to/input_mesh.vtk output_name`

**Run the Parallel Version (e.g., on 4 cores):**
`mpiexec -n 4 ./MeshCalculusOpenMPI path/to/input_mesh.vtk output_name`

Results will be saved in an `output/` directory automatically created adjacent to your project root.