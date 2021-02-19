# From OpenBLASConfig.cmake
#
# OpenBLASConfig.cmake
# --------------------
#
# OpenBLAS cmake module.
# This module sets the following variables in your project::
#
#   OpenBLAS_FOUND - true if OpenBLAS and all required components found on the system
#   OpenBLAS_VERSION - OpenBLAS version in format Major.Minor.Release
#   OpenBLAS_INCLUDE_DIRS - Directory where OpenBLAS header is located.
#   OpenBLAS_INCLUDE_DIR - same as DIRS
#   OpenBLAS_LIBRARIES - OpenBLAS library to link against.
#   OpenBLAS_LIBRARY - same as LIBRARIES
#

find_package(OpenBLAS COMPONENTS static)

if(NOT OpenBLAS_FOUND)
  # try to find by pkg-config if not found
  pkg_check_modules(OpenBLAS openblas)

  if (OpenBLAS_FOUND)
    # Pkg-config does not give absolute path to lib (i.e. OpenBLAS_LIBRARY)
    # Below is a fudge. In the pc file there happens to be
    # only one OpenBLAS_LIBRARY_DIRS, so it works.
    set(OpenBLAS_LIBRARY "${OpenBLAS_LIBRARY_DIRS}/libopenblas.a")

  else() #if not found by CMake or pkg-config...
    set(LDSCTRLEST_BUILD_STATIC FALSE)
    message(FATAL_ERROR "OpenBLAS not found by cmake or pkg-config. \
      Check CMAKE_PREFIX_PATH and PKG_CONFIG_PATH environment variables.")
  endif()#if found with pkg-config

endif()#if found with cmake

# Also have to link libgfortran for this work unfortunately.
if(OpenBLAS_FOUND)

  if(UNIX)
    set(LD_LIBRARY_PATH_ENV "$ENV{LD_LIBRARY_PATH}")

    if(NOT DEFINED ENV{LD_LIBRARY_PATH})
      message(STATUS "LD_LIBRARY_PATH environment variable not defined.")
    endif()

    set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${LD_LIBRARY_PATH_ENV}")
    # string(REPLACE <match-string> <replace-string> <out-var> <input>...)
    string(REPLACE ":" ";" LD_LIBRARY_PATH "${LD_LIBRARY_PATH}")
    message(STATUS "LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}")

    find_library(GFortran_LIBRARY NAMES gfortran libgfortran libgfortran.so libgfortran.dylib PATHS ${LD_LIBRARY_PATH})
  else()
    #assuming windows...
    #Not sure if PATH here is necessary.
    find_library(GFortran_LIBRARY NAMES gfortran libgfortran libgfortran.dll PATHS $ENV{PATH})
  endif()

  if(NOT GFortran_LIBRARY)
    message(FATAL_ERROR "GFortran library not found. Is the path to this listed in LD_LIBRARY_PATH (Unix) or PATH (Windows) environment variables?")
    set(OpenBLAS_FOUND FALSE)
    set(LDSCTRLEST_BUILD_STATIC FALSE)
  endif()

endif()

if(OpenBLAS_FOUND)

  list(APPEND PROJECT_REQUIRED_INCLUDE_DIRS ${OpenBLAS_INCLUDE_DIRS})
  list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME ${OpenBLAS_LIBRARY} ${GFortran_LIBRARY})

endif()

message(STATUS "OpenBLAS_INCLUDE_DIRS = ${OpenBLAS_INCLUDE_DIRS}")
message(STATUS "OpenBLAS_LIBRARIES    = ${OpenBLAS_LIBRARY};${GFortran_LIBRARY}")

