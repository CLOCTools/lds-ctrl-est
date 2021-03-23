# ``Matlab_FOUND``
#   ``TRUE`` if the Matlab installation is found, ``FALSE``
#
#   :command:`matlab_add_mex`
# adds a target compiling a MEX file.

# matlab_add_mex(
#          NAME <name>
#          [EXECUTABLE | MODULE | SHARED]
#          SRC src1 [src2 ...]
#          [OUTPUT_NAME output_name]
#          [DOCUMENTATION file.txt]
#          [LINK_TO target1 target2 ...]
#          [R2017b | R2018a]
#          [EXCLUDE_FROM_ALL]
#          [...]
#      )

matlab_add_mex(NAME glds_ssid_matlab SHARED SRC glds_ssid_mex.cpp OUTPUT_NAME "glds_ssid_mex" R2018a)
target_link_libraries(glds_ssid_matlab ${CMAKE_PROJECT_NAME}Static)
target_include_directories(glds_ssid_matlab PUBLIC ${Matlab_INCLUDE_DIRS})
target_include_directories(glds_ssid_matlab INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)

matlab_add_mex(NAME glds_em_refit_matlab SHARED SRC glds_em_refit_mex.cpp OUTPUT_NAME "glds_em_refit_mex" R2018a)
target_link_libraries(glds_em_refit_matlab ${CMAKE_PROJECT_NAME}Static)
target_include_directories(glds_em_refit_matlab PUBLIC ${Matlab_INCLUDE_DIRS})
target_include_directories(glds_em_refit_matlab INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)

matlab_add_mex(NAME plds_ssid_matlab SHARED SRC plds_ssid_mex.cpp OUTPUT_NAME "plds_ssid_mex" R2018a)
target_link_libraries(plds_ssid_matlab ${CMAKE_PROJECT_NAME}Static)
target_include_directories(plds_ssid_matlab PUBLIC ${Matlab_INCLUDE_DIRS})
target_include_directories(plds_ssid_matlab INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)

matlab_add_mex(NAME plds_em_refit_matlab SHARED SRC plds_em_refit_mex.cpp OUTPUT_NAME "plds_em_refit_mex" R2018a)
target_link_libraries(plds_em_refit_matlab ${CMAKE_PROJECT_NAME}Static)
target_include_directories(plds_em_refit_matlab PUBLIC ${Matlab_INCLUDE_DIRS})
target_include_directories(plds_em_refit_matlab INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)
