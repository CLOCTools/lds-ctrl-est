cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# set project name and version (MAJOR.minor.patch)
# for project name, use lowercase
project(test VERSION 0.1.0 LANGUAGES CXX C)
# some name variants I will be using:
set(CMAKE_PROJECT_NAME_CAP TEST)#all caps

# Specify C++ standard
# n.b. this has to be before adding targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

include(CheckIncludeFileCXX)
include(CheckLibraryExists)

# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)

############ OPTIONS
####################
# For usage later with clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

message(STATUS "CMAKE_SYSTEM_NAME          = ${CMAKE_SYSTEM_NAME}"         )
message(STATUS "CMAKE_CXX_COMPILER_ID      = ${CMAKE_CXX_COMPILER_ID}"     )
message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_COMPILER_IS_GNUCXX   = ${CMAKE_COMPILER_IS_GNUCXX}"  )
message(STATUS "")
message(STATUS "*** Looking for external libraries")

############ FIND LIBRARIES
###########################
# Find out package.Should return ldsCtrlEst.
find_package(ldsCtrlEst REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LDSCTRLEST_CXX_FLAGS}")

message(STATUS "CMAKE_CXX_FLAGS           = ${CMAKE_CXX_FLAGS}"          )
message(STATUS "CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")


####### Add targets
###################
# e.g., add executable
add_executable(test src/main.cpp)
target_link_libraries(test PUBLIC ldsCtrlEst ${LDSCTRLEST_LIBRARIES})
target_link_directories(test PUBLIC ${LDSCTRLEST_DIRS})
target_include_directories(test PUBLIC ${LDSCTRLEST_INCLUDE_DIRS})

# # dump all variables.
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

