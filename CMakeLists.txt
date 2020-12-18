cmake_minimum_required(VERSION 3.12.4)
project(Practice2)

set(CMAKE_CXX_STANDARD 20)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR} src)
add_subdirectory(src)

add_executable(Tests gtests/tests.cpp)
target_link_libraries(Tests Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} srcLib)

set(CMAKE_EXE_LINKER_FLAGS --coverage)

add_definitions(-fprofile-arcs -ftest-coverage)
target_link_libraries(Tests -fprofile-arcs)

add_executable(Practice main.cpp)
target_link_libraries(Practice srcLib)
