cmake_minimum_required(VERSION 3.8)
project(voxelizer_ros)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(sensor_msgs REQUIRED)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../cpp/voxelizer
  ${CMAKE_CURRENT_BINARY_DIR}/voxelizer_cpp
)

set(
  HEADER
  include/voxelizer_ros/voxelizer_node.hpp
)

set(
  SOURCE
  src/voxelizer_node.cpp
)

include_directories(
  include
  ${PCL_INCLUDE_DIRS}
)

add_executable(voxelizer_node ${SOURCE} ${HEADER})

ament_target_dependencies(
  voxelizer_node
  rclcpp 
  pcl_ros
  pcl_conversions 
  sensor_msgs
)

target_link_libraries(
  voxelizer_node
  voxelizer
  ${PCL_LIBRARIES}
)

install(
  TARGETS voxelizer_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
