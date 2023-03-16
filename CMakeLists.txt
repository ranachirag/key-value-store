if(UNIX AND NOT APPLE)
  cmake_minimum_required(VERSION 3.16)
else()
  cmake_minimum_required(VERSION 3.24)
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(kvstore)

include_directories(include)
include_directories(src)

add_subdirectory(src)
add_subdirectory(tst)