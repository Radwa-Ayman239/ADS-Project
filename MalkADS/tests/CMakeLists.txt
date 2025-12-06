cmake_minimum_required(VERSION 3.10)
project(tests)

# Download Catch2 via FetchContent
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(catch2)
# Add your test executable
add_executable(tests
    BooksManagerTester.cpp
    RoomsManagerTester.cpp
    LaptopsManagerTester.cpp
    UserManagerTester.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(tests
    PRIVATE
        MalkADS_lib
        Catch2::Catch2WithMain
)

# Automatically register tests with CTest
include(Catch)
catch_discover_tests(tests)
