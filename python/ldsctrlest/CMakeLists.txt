message(STATUS "*********************** Hello there from ldsctrlest!")

pybind11_add_module(example MODULE example.cpp)
pybind11_add_module(base MODULE base.cpp)
pybind11_add_module(gaussian MODULE gaussian.cpp)

target_compile_definitions(example PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_compile_definitions(base PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_compile_definitions(gaussian PRIVATE VERSION_INFO=${PROJECT_VERSION})

target_link_libraries(base PUBLIC ${CMAKE_PROJECT_NAME})
target_link_libraries(gaussian PUBLIC ${CMAKE_PROJECT_NAME})
# target_link_libraries(base PRIVATE ${PROJECT_REQUIRED_LIBRARIES})
# target_link_libraries(gaussian PRIVATE ${PROJECT_REQUIRED_LIBRARIES})