find_package(Armadillo "9" REQUIRED)

list(APPEND PROJECT_REQUIRED_INCLUDE_DIRS ${ARMADILLO_INCLUDE_DIRS})

# have made hdf5 a required dependency
set(PROJECT_REQUIRED_CXX_FLAGS "${PROJECT_REQUIRED_CXX_FLAGS} -DARMA_USE_HDF5")

if ((LDSCTRLEST_BUILD_STATIC AND NOT APPLE) OR (WIN32))
  message(STATUS "Not using armadillo wrapper.")
  set(PROJECT_REQUIRED_CXX_FLAGS "${PROJECT_REQUIRED_CXX_FLAGS} -DARMA_DONT_USE_WRAPPER")
  set(ARMADILLO_LIBRARIES "")
endif()
list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME ${ARMADILLO_LIBRARIES})

# TODO(mfbolus): seems that there is a random number generator issue with apple clang?
# NB: this was resolved in commit () but keeping it for now.
if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(PROJECT_REQUIRED_CXX_FLAGS "${PROJECT_REQUIRED_CXX_FLAGS} -DARMA_DONT_USE_EXTERN_RNG")
endif()

if(MINGW OR MSYS OR CYGWIN OR MSVC)
  # According to Armadillo: MinGW doesn't correctly handle thread_local
  # therefore,
  set(PROJECT_REQUIRED_CXX_FLAGS "${PROJECT_REQUIRED_CXX_FLAGS} -DARMA_DONT_USE_EXTERN_RNG")
endif()

message(STATUS "ARMADILLO_INCLUDE_DIRS = ${ARMADILLO_INCLUDE_DIRS}")
message(STATUS "ARMADILLO_LIBRARIES    = ${ARMADILLO_LIBRARIES}")

## COMPILER COMPAT CHECK TAKEN FROM ARMADILLO 10.2.x. ALL CREDIT TO ORIGINAL AUTHORS.
# NOTE: ARMA_USE_EXTERN_RNG requires compiler support for thread_local and C++11
# NOTE: for Linux, this is available with gcc 4.8.3 onwards
# NOTE: for macOS, thread_local is supoported in Xcode 8 (mid 2016 onwards) in C++11 mode
if(DEFINED CMAKE_CXX_COMPILER_ID AND DEFINED CMAKE_CXX_COMPILER_VERSION)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(NOT (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 4.8.3))
      set(ARMA_USE_EXTERN_RNG true)
      # message(STATUS "Detected gcc 4.8.3 or later")
      if(NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        message(STATUS "Added '-std=c++11' to compiler flags")
      endif()
    else()
      message(FATAL_ERROR "Compiler too old")
    endif()
  else()
    if(NOT (${CMAKE_MAJOR_VERSION} LESS 3))
      if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 6.0)
          set(ARMA_USE_EXTERN_RNG true)
          # message(STATUS "Detected Clang 6.0 or later")
          if(NOT DEFINED CMAKE_CXX_STANDARD)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
            message(STATUS "Added '-std=c++14' to compiler flags")
          endif()
        else()
          message(FATAL_ERROR "Compiler too old")
        endif()
      elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        if(NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 8.0)
          set(ARMA_USE_EXTERN_RNG true)
          # message(STATUS "Detected AppleClang 8.0 or later")
          if(NOT DEFINED CMAKE_CXX_STANDARD)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
            message(STATUS "Added '-std=c++14' to compiler flags")
          endif()
        else()
          message(FATAL_ERROR "Compiler too old")
        endif()
      endif()
    endif()
  endif()
endif()
