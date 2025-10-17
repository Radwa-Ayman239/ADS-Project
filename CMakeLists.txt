cmake_minimum_required(VERSION 4.0)
project(ADS_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(ADS_Project
        include/hash_map.h
        src/hash_map.cpp)
