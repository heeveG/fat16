cmake_minimum_required(VERSION 3.17)
project(fat16)

set(CMAKE_CXX_STANDARD 14)

add_executable(fat16 main.cpp utils/utils.cpp utils/utils.h utils/dirUtils.cpp utils/dirUtils.h)