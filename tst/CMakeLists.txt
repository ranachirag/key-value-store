set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

enable_testing()
    
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(NAME ${BINARY} COMMAND ${BINARY})