# add executables
add_executable(test_glds_ctrl test_glds_ctrl.cpp)
target_link_libraries(test_glds_ctrl PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})

add_executable(test_plds_est test_plds_est.cpp)
target_link_libraries(test_plds_est PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})

add_executable(test_plds_logLinCtrl test_plds_logLinCtrl.cpp)
target_link_libraries(test_plds_logLinCtrl PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})

add_executable(test_plds_switched_logLinCtrl test_plds_switched_logLinCtrl.cpp)
target_link_libraries(test_plds_switched_logLinCtrl PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})
