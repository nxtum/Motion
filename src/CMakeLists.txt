add_library(motion)
add_library(Motion::Motion ALIAS motion)
target_compile_features(
    motion
    PUBLIC
        cxx_std_26
)

set_target_properties(
    motion
    PROPERTIES
        CXX_EXTENSIONS OFF
)

target_sources(
    motion
    PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}"
        FILES
            motion/motion.cppm
            motion/geometry/geometry.cppm
            motion/graph/graph.cppm
            motion/grid/grid.cppm
            motion/path/path.cppm
            motion/planning/planning.cppm
)

motion_set_project_warnings(motion)
