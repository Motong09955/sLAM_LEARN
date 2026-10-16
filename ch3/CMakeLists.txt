cmake_minimum_required(VERSION 3.10)
project(sLAM_LEARN)


set(CMAKE_CXX_FLAGS "-std=c++11")

# 加入各子工程
add_subdirectory(useEigen)
add_subdirectory(useGeometry)
add_subdirectory(examples)