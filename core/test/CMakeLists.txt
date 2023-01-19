cmake_minimum_required(VERSION 3.16)
project(mt_test)

add_executable(test test.cpp ../mt_core.hpp ../mt_msg.hpp)
target_link_libraries(test pthread)