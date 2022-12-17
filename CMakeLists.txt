cmake_minimum_required(VERSION 3.23)
project(parsers)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

add_executable(
        parsers
        main.cpp
        src/Rule.cpp
        src/Grammar.cpp
        src/ParserCYK.cpp)

target_include_directories(parsers PUBLIC ${CMAKE_SOURCE_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(parsers parsers_lib)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(
        Test
        ${CMAKE_SOURCE_DIR}/tests/main_tests.cpp
        ${CMAKE_SOURCE_DIR}/tests/tests.cpp
)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(Test PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(Test parsers_lib)
target_link_libraries(Test ${GTEST_LIBRARIES} Threads::Threads)
add_test(Test ${CMAKE_SOURCE_DIR}/Tested)

set(CMAKE_EXE_LINKER_FLAGS --coverage)

add_definitions(-fprofile-arcs -ftest-coverage)
target_link_libraries(Test -fprofile-arcs)

