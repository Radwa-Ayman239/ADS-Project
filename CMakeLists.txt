cmake_minimum_required(VERSION 4.0)
project(ADS_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(ADS_Project
        include/hash_map.h
        tests/hash_test.cpp
        include/models/user.h
        include/managers/UserManager.h
        src/models/User.cpp
        src/managers/LibrarySystem.cpp
        include/managers/LibrarySystem.h
        src/managers/UserManager.cpp
)
