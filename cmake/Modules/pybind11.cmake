add_subdirectory(python/pybind11)
if (Python_FOUND OR PYTHON_EXECUTABLE)
    set(Python_FOUND TRUE)
endif()