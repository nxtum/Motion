add_executable(
    motion_smoke
    smoke/main.cpp
)

target_link_libraries(
    motion_smoke
    PRIVATE
        Motion::Motion
)

motion_set_project_warnings(motion_smoke)