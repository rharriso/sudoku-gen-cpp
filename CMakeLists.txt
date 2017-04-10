cmake_minimum_required(VERSION 3.5)
project(sudoku_solver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

set(SOURCE_FILES main.cpp)
add_executable(sudoku_generator_cpp ${SOURCE_FILES})