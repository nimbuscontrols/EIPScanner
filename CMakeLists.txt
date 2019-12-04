cmake_minimum_required(VERSION 3.5)
project(EIPScanner)

set(CMAKE_CXX_STANDARD 14)
option(TEST_ENABLED "Enable unit test" OFF)
option(EXAMPLE_ENABLED "Build examples" OFF)

add_subdirectory(src)
if (EXAMPLE_ENABLED)
    add_subdirectory(examples)
endif()

if (TEST_ENABLED)
    add_subdirectory(test)
endif ()