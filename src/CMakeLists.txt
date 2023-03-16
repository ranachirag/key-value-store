set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})


add_executable(${BINARY}_run ${SOURCES})
target_compile_features(${BINARY}_run PRIVATE cxx_std_17)

find_path(XXHASH_INCLUDE_DIR xxhash.h)
find_library(XXHASH_LIBRARY xxhash)
include_directories(${XXHASH_INCLUDE_DIR})
link_libraries(${XXHASH_LIBRARY})


add_library(${BINARY}_lib STATIC ${SOURCES})