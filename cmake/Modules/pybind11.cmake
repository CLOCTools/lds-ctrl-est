# these are needed for CARMA
find_package(Python3 COMPONENTS Interpreter Development NumPy)
add_subdirectory(python/pybind11)
# to make compatible for install:
if(LDSCTRLEST_INSTALL)
  # need to use installed version of carma which has export set defined
  FIND_PACKAGE(carma CONFIG REQUIRED)
else()
  add_subdirectory(python/carma)
endif()
# carma and Python must be linked to ldsCtrlEst, not just binding modules
# since carma needs to be able to change how Armadillo manages memory
list(APPEND PROJECT_REQUIRED_LIBRARIES_ABSOLUTE_NAME carma::carma Python3::Python)
# in case you pass in PYTHON_EXECUTABLE directly
if (Python3_FOUND OR PYTHON_EXECUTABLE)
  set(Python_FOUND TRUE)
endif()