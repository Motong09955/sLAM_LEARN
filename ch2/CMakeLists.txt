cmake_minimum_required (VERSION 3.22)

project ( HELLOSLAM )

add_executable ( useHello useHello.cpp )
target_link_libraries(useHello hello_shared)
//这里的targetlink要放在add_executable之后

add_library(hello_shared SHARED libHelloSLAM.cpp)