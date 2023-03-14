cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

project(kvstore)

include_directories(include)
include_directories(src)

add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(lib/googletest)