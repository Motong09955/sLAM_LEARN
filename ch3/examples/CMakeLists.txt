cmake_minimum_required(VERSION 3.10)
project(plot)

include_directories("/usr/include/eigen3")

add_executable(eigenExample coordinateTransform.cpp)

find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})

add_executable(plot plotTrajectory.cpp)
target_link_libraries(plot ${Pangolin_LIBRARIES})