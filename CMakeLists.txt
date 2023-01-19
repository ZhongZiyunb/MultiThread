cmake_minimum_required(VERSION 3.16)

project(test)
set(CMAKE_CXX_FLAGS "-fno-elide-constructors ${CMAKE_CXX_FLAGS}")
add_executable(test main.cpp)