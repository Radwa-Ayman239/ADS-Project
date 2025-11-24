cmake_minimum_required(VERSION 3.10)

project(MalkADS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Original executable
add_executable(MalkADS
        main.cpp

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
        src/models/user.cpp
        include/helpers/ResourceIO.h
        include/helpers/UIHelpers.h      # optional but you probably want it too
)

# Python module using pybind11
find_package(Python COMPONENTS Interpreter Development)
if(Python_FOUND)
    # Try to find pybind11
    find_package(pybind11 CONFIG QUIET)
    
    if(pybind11_FOUND)
        pybind11_add_module(library_system bindings.cpp)
        
        # Set the output directory to the project root for easy import
        set_target_properties(library_system PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}"
        )
        
        message(STATUS "Python module will be built")
    else()
        message(WARNING "pybind11 not found. Python module will not be built. Install with: pip install pybind11")
    endif()
else()
    message(WARNING "Python development files not found. Python module will not be built.")
endif()

