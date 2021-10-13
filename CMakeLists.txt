cmake_minimum_required(VERSION 3.20)
project(Innovation_PL)

set(CMAKE_CXX_STANDARD 17)

add_executable(Innovation_PL main.cpp src/common.h src/chunk.h src/debug.h src/debug.cpp src/value.h src/value.cpp src/chunk.cpp src/vm.h src/vm.cpp)
