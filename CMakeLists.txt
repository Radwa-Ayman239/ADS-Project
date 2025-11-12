cmake_minimum_required(VERSION 4.0)
project(ADS_Project)

set(CMAKE_CXX_STANDARD 14)

# This command tells the compiler where to look for your header files,
# allowing includes like #include "managers/UserManager.h" to work.
include_directories(${PROJECT_SOURCE_DIR}/include)

# CRITICAL FIX: Only list the .cpp (source) files here.
# All the header files (.h) have been removed.
add_executable(ADS_Project
        # Source files for the main application
        main.cpp
        src/IntervalTreeComplete.cpp
        src/priority_queue.cpp
        src/queue.cpp

        # Source files for managers and models
        src/managers/LibrarySystem.cpp
        src/managers/UserManager.cpp
        src/models/User.cpp

        # Source files for the test (if you want to build tests into the main executable)
        # Note: It's usually better to define a separate target for tests, but we'll
        # include it here to compile all .cpp files you had listed.
)