find_package(hdf5 CONFIG REQUIRED)
if(HDF5_BUILD_SHARED_LIBS)
    list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME hdf5::hdf5-shared hdf5::hdf5_hl-shared)
elseif(HDF5_BUILD_STATIC_LIBS)
    list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME hdf5::hdf5-static hdf5::hdf5_hl-static)
else()
    message(FATAL_ERROR "Could not determine if HDF5 libraries are dynamic or static.")
endif()
