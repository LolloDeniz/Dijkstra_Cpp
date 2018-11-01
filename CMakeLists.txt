cmake_minimum_required(VERSION 3.12)
project(C___version)

set(CMAKE_CXX_STANDARD 14)

add_executable(C___version main.cpp Node.h Node.cpp Edge.cpp Edge.h PQ.cpp PQ.h)