message(STATUS "*********************** Hello there from ldsctrlest!")

pybind11_add_module(example MODULE example.cpp)

target_compile_definitions(example PRIVATE VERSION_INFO=${PROJECT_VERSION})