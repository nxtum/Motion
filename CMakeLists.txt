cmake_minimum_required(VERSION 4.0)

project(
    Motion
    VERSION 0.1.0
    DESCRIPTION "Modern motion-planning library"
    LANGUAGES CXX
)

option(MOTION_BUILD_EXAMPLES "Build Motion examples" ON)
option(MOTION_BUILD_TESTS "Build Motion tests" ON)

include(cmake/CompilerWarnings.cmake)

add_subdirectory(src)

if(MOTION_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(MOTION_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()