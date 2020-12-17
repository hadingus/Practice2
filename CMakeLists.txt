cmake_minimum_required(VERSION 3.12.4)
project(Practice2)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(src)
add_executable(Practice main.cpp)
target_link_libraries(Practice srcLib)