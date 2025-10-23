cmake_minimum_required(VERSION 3.28.3)
project(c-mat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(c-mat
	src/interpolate/taylor.cpp
)

target_include_directories(c-mat
	PUBLIC
		${CMAKE_SOURCE_DIR}/include
)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE c-mat)
