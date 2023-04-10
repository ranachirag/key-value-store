set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_step3_exp2 ${SOURCES})

target_link_libraries(${BINARY}_step3_exp2 PRIVATE ${BINARY}_lib)

