# MODIFED FROM ORIGINAL IN ARMADILLO LIBRARY. ALL CREDIT TO ORIGINAL AUTHORS. 
# 

# - Find the MKL libraries (no includes)
# This module defines
#  MKL_LIBRARIES, the libraries needed to use Intel's implementation of BLAS & LAPACK.
#  MKL_FOUND, If false, do not try to use MKL.

## the link below explains why we're linking only with mkl_rt
## https://software.intel.com/en-us/articles/a-new-linking-model-single-dynamic-library-mkl_rt-since-intel-mkl-103

set(MKL_NAMES ${MKL_NAMES} mkl_rt)

# Explicitly link? 
# see: https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/onemkl/link-line-advisor.html
# 
#set(MKL_NAMES ${MKL_NAMES} mkl_intel_ilp64_dll)
#set(MKL_NAMES ${MKL_NAMES} mkl_intel_thread_dll)
#set(MKL_NAMES ${MKL_NAMES} mkl_core_dll)
#set(MKL_NAMES ${MKL_NAMES} libiomp5md)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(MKL_ARCH intel64)
else()
  set(MKL_ARCH ia32)
endif()

# set(MKL_ROOT $ENV{MKLROOT} CACHE TYPE STRING)
# message(STATUS "MKL_ROOT = ${MKL_ROOT}")
# 
# if(NOT MKL_ROOT)
#   set(MKL_ROOT "/opt/intel/oneapi/mkl")
#   message(STATUS "NOTE: Assuming MKL_ROOT = ${MKL_ROOT}")
# endif()

set(ONEAPI_ROOT $ENV{ONEAPI_ROOT})
message(STATUS "ONEAPI_ROOT = ${ONEAPI_ROOT}")

if(NOT ONEAPI_ROOT)
  set(ONEAPI_ROOT "/opt/intel/oneapi")
  message(STATUS "NOTE: Assuming ONEAPI_ROOT = ${ONEAPI_ROOT}")
endif()

set(MKL_ROOT "${ONEAPI_ROOT}mkl")
message(STATUS "MKL_ROOT = ${MKL_ROOT}")

foreach (MKL_NAME ${MKL_NAMES})
  find_library(${MKL_NAME}_LIBRARY
    NAMES ${MKL_NAME}
    PATHS
    ${CMAKE_SYSTEM_LIBRARY_PATH}
    ${MKL_ROOT}/lib/${MKL_ARCH}
    ${MKL_ROOT}/latest/lib/${MKL_ARCH}
    ${MKL_ROOT}
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/intel/composerxe/lib/intel64
    /opt/intel/composerxe/lib/ia32
    /opt/intel/composerxe/lib/mkl/lib/intel64
    /opt/intel/composerxe/lib/mkl/lib/ia32
    /usr/local/intel/composerxe/lib/intel64
    /usr/local/intel/composerxe/lib/ia32
    /usr/local/intel/composerxe/lib/mkl/lib/intel64
    /usr/local/intel/composerxe/lib/mkl/lib/ia32
    /opt/intel/lib
    /opt/intel/lib/intel64
    /opt/intel/lib/em64t
    /opt/intel/lib/lib64
    /opt/intel/lib/ia32
    /opt/intel/mkl/lib
    /opt/intel/mkl/lib/intel64
    /opt/intel/mkl/lib/em64t
    /opt/intel/mkl/lib/lib64
    /opt/intel/mkl/lib/ia32
    /opt/intel/mkl/*/lib
    /opt/intel/mkl/*/lib/intel64
    /opt/intel/mkl/*/lib/em64t
    /opt/intel/mkl/*/lib/lib64
    /opt/intel/mkl/*/lib/32
    /opt/intel/*/mkl/lib
    /opt/intel/*/mkl/lib/intel64
    /opt/intel/*/mkl/lib/em64t
    /opt/intel/*/mkl/lib/lib64
    /opt/intel/*/mkl/lib/ia32
    /opt/mkl/lib
    /opt/mkl/lib/intel64
    /opt/mkl/lib/em64t
    /opt/mkl/lib/lib64
    /opt/mkl/lib/ia32
    /opt/mkl/*/lib
    /opt/mkl/*/lib/intel64
    /opt/mkl/*/lib/em64t
    /opt/mkl/*/lib/lib64
    /opt/mkl/*/lib/32
    /usr/local/intel/lib
    /usr/local/intel/lib/intel64
    /usr/local/intel/lib/em64t
    /usr/local/intel/lib/lib64
    /usr/local/intel/lib/ia32
    /usr/local/intel/mkl/lib
    /usr/local/intel/mkl/lib/intel64
    /usr/local/intel/mkl/lib/em64t
    /usr/local/intel/mkl/lib/lib64
    /usr/local/intel/mkl/lib/ia32
    /usr/local/intel/mkl/*/lib
    /usr/local/intel/mkl/*/lib/intel64
    /usr/local/intel/mkl/*/lib/em64t
    /usr/local/intel/mkl/*/lib/lib64
    /usr/local/intel/mkl/*/lib/32
    /usr/local/intel/*/mkl/lib
    /usr/local/intel/*/mkl/lib/intel64
    /usr/local/intel/*/mkl/lib/em64t
    /usr/local/intel/*/mkl/lib/lib64
    /usr/local/intel/*/mkl/lib/ia32
    /usr/local/mkl/lib
    /usr/local/mkl/lib/intel64
    /usr/local/mkl/lib/em64t
    /usr/local/mkl/lib/lib64
    /usr/local/mkl/lib/ia32
    /usr/local/mkl/*/lib
    /usr/local/mkl/*/lib/intel64
    /usr/local/mkl/*/lib/em64t
    /usr/local/mkl/*/lib/lib64
    /usr/local/mkl/*/lib/32
    )

  set(TMP_LIBRARY ${${MKL_NAME}_LIBRARY})

  if(TMP_LIBRARY)
    set(MKL_LIBRARIES ${MKL_LIBRARIES} ${TMP_LIBRARY})
  endif()
endforeach()

if(MKL_LIBRARIES)
  set(MKL_FOUND "YES")
else()
  set(MKL_FOUND "NO")
endif()

if(MKL_FOUND)
# Add for linked libs commented above: -DMKL_ILP64
    set(PROJECT_REQUIRED_CXX_FLAGS "${PROJECT_REQUIRED_CXX_FLAGS} -DARMA_USE_LAPACK -DARMA_USE_BLAS")
    list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME ${MKL_LIBRARIES})
    message(STATUS "Found MKL libraries: ${MKL_LIBRARIES}")
else()
    message(FATAL_ERROR "Could not find MKL libraries")
endif()

