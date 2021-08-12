# these are needed for CARMA
find_package(Python3 COMPONENTS Interpreter Development NumPy)
add_subdirectory(python/pybind11)
add_subdirectory(python/carma)
# in case you pass in PYTHON_EXECUTABLE directly
if (Python3_FOUND OR PYTHON_EXECUTABLE)
    set(Python_FOUND TRUE)
endif()