cmake_minimum_required(VERSION 2.8)
project(biganalyze)


#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp  -m64 -msse4.1 -O3 -I/home/jiang/home  -std=c++11")
add_subdirectory(Collaborative_Filtering)
add_subdirectory(MF)
