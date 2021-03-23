# ``Matlab_FOUND``
#   ``TRUE`` if the Matlab installation is found, ``FALSE``
#
#   ``Matlab_INCLUDE_DIRS``
# the path of the Matlab libraries headers
#
#   :command:`matlab_add_mex`
# adds a target compiling a MEX file.

set(MATLAB_ADDITIONAL_VERSIONS "R2021a=9.10")

find_package(Matlab)

# if (Matlab_FOUND)
# list(APPEND PROJECT_REQUIRED_INCLUDE_DIRS ${Matlab_INCLUDE_DIRS})
# endif()
