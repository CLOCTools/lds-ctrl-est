find_package(hdf5 CONFIG REQUIRED)
list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME hdf5::hdf5-shared hdf5::hdf5_hl-shared)
