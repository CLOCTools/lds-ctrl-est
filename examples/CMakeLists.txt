# add executables
add_executable(eg_glds_ctrl eg_glds_ctrl.cpp)
target_link_libraries(eg_glds_ctrl PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})

add_executable(eg_glds_du_plds_ctrl eg_glds_du_plds_ctrl.cpp)
target_link_libraries(eg_glds_du_plds_ctrl PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})

add_executable(eg_plds_est eg_plds_est.cpp)
target_link_libraries(eg_plds_est PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})

add_executable(eg_plds_ctrl eg_plds_ctrl.cpp)
target_link_libraries(eg_plds_ctrl PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})

add_executable(eg_plds_switched_ctrl eg_plds_switched_ctrl.cpp)
target_link_libraries(eg_plds_switched_ctrl PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_LIBS})
