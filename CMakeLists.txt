cmake_minimum_required(VERSION 3.15)
project(EIPScanner)

set(CMAKE_CXX_STANDARD 14)
option(TEST_ENABLED "Enable unit test" OFF)

add_subdirectory(src)
if (TEST_ENABLED)
    add_subdirectory(test)
endif ()