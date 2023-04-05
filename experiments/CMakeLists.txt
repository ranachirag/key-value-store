set(BINARY ${CMAKE_PROJECT_NAME}_experiment)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})


add_executable(${BINARY}_run ${SOURCES})

find_path(XXHASH_INCLUDE_DIR xxhash.h)
find_library(XXHASH_LIBRARY xxhash)
include_directories(${XXHASH_INCLUDE_DIR})
link_libraries(${XXHASH_LIBRARY})

find_package(xxHash CONFIG REQUIRED)
target_link_libraries(${BINARY}_run PRIVATE xxHash::xxhash)


add_library(${BINARY}_lib STATIC ${SOURCES})