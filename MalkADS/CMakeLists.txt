# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(MalkADS)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable and list all your cpp files
add_executable(MalkADS
    main.cpp
    Users.cpp
    Rooms.cpp
    Laptops.cpp
    Books.cpp
    IntervalTreeComplete.cpp
)
