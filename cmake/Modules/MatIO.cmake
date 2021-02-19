# If we had a cmake config file for matio:
# find_package(matio REQUIRED)

# in general, we don't have cmake configs for this
# use pkg-config
pkg_check_modules(MATIO matio REQUIRED)
list(APPEND PROJECT_REQUIRED_INCLUDE_DIRS ${MATIO_INCLUDE_DIRS})
list(APPEND PROJECT_REQUIRED_LIBRARY_DIRS ${MATIO_LIBRARY_DIRS})
list(APPEND PROJECT_REQUIRED_LIBRARIES_SHORT_NAME ${MATIO_LIBRARIES})

message(STATUS "MATIO_INCLUDE_DIRS = ${MATIO_INCLUDE_DIRS}")
message(STATUS "MATIO_LIBRARY_DIRS = ${MATIO_LIBRARY_DIRS}")
message(STATUS "MATIO_LIBRARIES = ${MATIO_LIBRARIES}")

