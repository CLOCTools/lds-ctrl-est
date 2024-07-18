include(FetchContent)
message(STATUS "Fetching & installing osqp")
FetchContent_Declare(
    osqp
    PREFIX ${CMAKE_BINARY_DIR}/osqp
    GIT_REPOSITORY https://github.com/osqp/osqp.git
    GIT_TAG 4e81a9d0
)
FetchContent_MakeAvailable(osqp)
message(STATUS "Installed osqp to ${osqp_BINARY_DIR}")
list(APPEND CMAKE_PREFIX_PATH ${osqp_BINARY_DIR})
find_package(osqp REQUIRED)
list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME osqp::osqp)