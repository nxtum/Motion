include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.2
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(Catch2)

add_executable(
    motion_tests
    tests_grid.cpp
)

target_link_libraries(
    motion_tests
    PRIVATE
        Motion::Motion
        Catch2::Catch2WithMain
)

motion_set_project_warnings(motion_tests)

list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(Catch)
catch_discover_tests(motion_tests)