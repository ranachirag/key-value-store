cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)


project(kvstore)

include_directories("include/")

set(SRC_DIR "src/")

file(GLOB SRCFILES "${SRC_DIR}/*.cpp")
list(APPEND SRCFILES main.cpp)
# create executable
include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(${PROJECT_NAME} ${SRCFILES})