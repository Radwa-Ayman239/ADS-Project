cmake_minimum_required(VERSION 3.10)

project(MalkADS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ---------------------------------------
# Library with core source files
# ---------------------------------------
add_library(MalkADS_lib
        # managers
        src/managers/LibrarySystem.cpp
        src/managers/UsersManager.cpp
        src/managers/RoomsManager.cpp
        src/managers/LaptopsManager.cpp
        src/managers/BooksManager.cpp

        # models
        src/models/user.cpp
        src/models/book.cpp

        # structures
        src/structures/IntervalTreeComplete.cpp
        src/structures/queue.cpp
)

# Include headers
target_include_directories(MalkADS_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# ---------------------------------------
# Main executable
# ---------------------------------------
add_executable(MalkADS main.cpp)
target_link_libraries(MalkADS PRIVATE MalkADS_lib)

# ---------------------------------------
# Python module using pybind11
# ---------------------------------------
find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG QUIET)

if(Python_FOUND)
    

    if(pybind11_FOUND)
        pybind11_add_module(library_system bindings.cpp)
        target_link_libraries(library_system PRIVATE MalkADS_lib)

        set_target_properties(library_system PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}"
        )

        message(STATUS "Python module will be built")

    else()
        message(WARNING "pybind11 not found. Python module will not be built.")
    endif()

else()
    message(WARNING "Python development files not found. Python module will not be built.")
endif()

# ---------------------------------------
# Testing
# ---------------------------------------
enable_testing()
add_subdirectory(tests)
