pybind11_add_module(base MODULE base.cpp)
pybind11_add_module(gaussian MODULE gaussian.cpp)
pybind11_add_module(poisson MODULE poisson.cpp)
add_custom_target(python_modules DEPENDS base gaussian poisson)

# Set version info for all modules
target_compile_definitions(base PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_compile_definitions(gaussian PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_compile_definitions(poisson PRIVATE VERSION_INFO=${PROJECT_VERSION})

# Link with main library
target_link_libraries(base PUBLIC ${CMAKE_PROJECT_NAME})
target_link_libraries(gaussian PUBLIC ${CMAKE_PROJECT_NAME})
target_link_libraries(poisson PUBLIC ${CMAKE_PROJECT_NAME})

# Set RPATH so the extension modules can find the main library
if(APPLE)
    set_target_properties(base gaussian poisson PROPERTIES
        INSTALL_RPATH "@loader_path"
        BUILD_WITH_INSTALL_RPATH TRUE)
else()
    set_target_properties(base gaussian poisson PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE)
endif()

# Install the Python modules
install(TARGETS base gaussian poisson
    LIBRARY DESTINATION ldsctrlest
    RUNTIME DESTINATION ldsctrlest)
