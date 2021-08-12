message(STATUS "*********************** Hello there from ldsctrlest!")

pybind11_add_module(example MODULE example.cpp)
pybind11_add_module(base MODULE base.cpp)
pybind11_add_module(gaussian MODULE gaussian.cpp)
add_custom_target(python_modules DEPENDS base gaussian)

# need C++14 for CARMA
set_property(TARGET base PROPERTY CXX_STANDARD 14)
set_property(TARGET base PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(example PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_compile_definitions(base PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_compile_definitions(gaussian PRIVATE VERSION_INFO=${PROJECT_VERSION})

target_link_libraries(example PUBLIC ${CMAKE_PROJECT_NAME})
target_link_libraries(base PUBLIC ${CMAKE_PROJECT_NAME})
target_link_libraries(gaussian PUBLIC ${CMAKE_PROJECT_NAME})

target_link_libraries(example PRIVATE carma)
target_link_libraries(base PRIVATE carma)
target_link_libraries(gaussian PRIVATE carma)