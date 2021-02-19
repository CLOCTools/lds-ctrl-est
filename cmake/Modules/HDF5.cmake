find_package(HDF5 COMPONENTS C)

if(HDF5_FOUND)
  list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME ${HDF5_LIBRARIES})
else()
  # try pkg-config
  if (PKG_CONFIG_FOUND)
    pkg_check_modules(HDF5 hdf5)

    if (HDF5_FOUND)
      list(APPEND PROJECT_REQUIRED_LIBRARY_DIRS ${HDF5_LIBRARY_DIRS})
      list(APPEND PROJECT_REQUIRED_LIBRARIES_SHORT_NAME ${HDF5_LIBRARIES})
    endif()

  endif()
endif()

if(HDF5_FOUND)
  list(APPEND PROJECT_REQUIRED_INCLUDE_DIRS ${HDF5_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "HDF5 was not found by cmake or pkg-config.")
endif()
