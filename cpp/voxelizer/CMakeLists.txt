cmake_minimum_required(VERSION 3.16...3.28)
project(voxelizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)
find_package(TBB REQUIRED)

set(
    SOURCES
    src/voxel_map.cpp
    src/voxel.cpp
)

set(
    HEADERS
    include/voxel.hpp
    include/voxel_map.hpp
)

include_directories(include)

add_library(voxelizer STATIC)
target_sources(voxelizer PRIVATE ${SOURCES} ${HEADERS})
target_include_directories(voxelizer PUBLIC include)
target_link_libraries(voxelizer PUBLIC Eigen3::Eigen  TBB::tbb)