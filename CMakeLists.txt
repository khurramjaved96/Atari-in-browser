cmake_minimum_required (VERSION 3.14)

# Project specific target properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

project(ale-wasm LANGUAGES CXX)

add_executable(ale examples/cpp-interface/sharedLibraryInterfaceExample.cpp)
add_subdirectory(src)

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(ale
        PUBLIC
        ZLIB::ZLIB
        Threads::Threads dl)
