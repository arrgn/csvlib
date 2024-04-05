cmake_minimum_required(VERSION 3.8)

project(csvlib)

set(CMAKE_CXX_STANDARD 17)

set(H_FILES src/csvlib.h)
set(CPP_FILES src/csvlib.cpp)

add_library(csvlib SHARED ${H_FILES} ${CPP_FILES})
