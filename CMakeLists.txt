cmake_minimum_required(VERSION 3.20)

project(vi-rt)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)
set(CXX_STANDARD C++20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

file(GLOB_RECURSE sources src/**/*.cpp src/*.cpp)

add_subdirectory(vendor/fmt)
add_subdirectory(vendor/tinyobjloader)

add_executable(vi-rt ${sources})

target_link_libraries(vi-rt fmt::fmt)
target_link_libraries(vi-rt tinyobjloader)

target_include_directories(vi-rt PRIVATE src)

