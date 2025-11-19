cmake_minimum_required(VERSION 3.10)

project(MalkADS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
        src/models/user.cpp      # optional but you probably want it too
)
