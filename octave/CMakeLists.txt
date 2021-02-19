# The macro octave_add_oct allows to create compiled modules.
# octave_add_oct ( target_name
#         [SOURCES] source1 [source2 ...]
#         [LINK_LIBRARIES  lib1 [lib2 ...]]
#         [EXTENSION ext]
# )
octave_add_oct(glds_ssid_mex glds_ssid_mex.cpp LINK_LIBRARIES ${CMAKE_PROJECT_NAME}Static ${OCTAVE_LIBRARIES} EXTENSION "mex")
target_include_directories(glds_ssid_mex PUBLIC ${OCTAVE_INCLUDE_DIRS})
target_include_directories(glds_ssid_mex INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)
