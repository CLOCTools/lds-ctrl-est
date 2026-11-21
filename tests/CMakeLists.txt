add_executable(lds_unit_tests
  test_main.cpp
  test_utils.cpp
  test_glds_sys.cpp
  test_plds_sys.cpp
  test_ctrl.cpp
  test_sctrl.cpp
  test_hdf5_io.cpp
)

target_include_directories(lds_unit_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_link_libraries(lds_unit_tests PUBLIC
  ${CMAKE_PROJECT_NAME}
  ${PROJECT_REQUIRED_LIBRARIES}
)

set(UNIT_SUITES
  "unit_utils:utils"
  "unit_glds_sys:glds_sys"
  "unit_plds_sys:plds_sys"
  "unit_ctrl:controller"
  "unit_sctrl:switched_controller"
  "unit_hdf5_io:hdf5_io"
)

foreach(entry ${UNIT_SUITES})
  string(REPLACE ":" ";" pair ${entry})
  list(GET pair 0 test_name)
  list(GET pair 1 suite_name)
  add_test(NAME ${test_name}
           COMMAND lds_unit_tests --test-suite=${suite_name} --no-intro=true)
  set_tests_properties(${test_name} PROPERTIES TIMEOUT 30)
endforeach()
