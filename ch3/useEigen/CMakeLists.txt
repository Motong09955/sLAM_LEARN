cmake_minimum_required(VERSION 3.10)
project(useEigen)

include_directories("/usr/include/eigen3")   # 关键的一行：告诉编译器 Eigen 在哪

add_executable(eigenMatrix eigenMatrix.cpp)