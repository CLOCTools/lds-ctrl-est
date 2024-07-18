# add executables


set(CTEST_BINARY_DIRECTORY ${PROJECT_BINARY_DIR}/tests)

file(GLOB files "eg_*.cpp")

foreach(file ${files})
	string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})
	add_executable(${file_without_ext} ${file})
	target_link_libraries(${file_without_ext} PUBLIC ${CMAKE_PROJECT_NAME} ${PROJECT_REQUIRED_LIBRARIES})
	add_test(${file_without_ext} ${file_without_ext})
	# set_tests_properties(${file_without_ext}
	# 	PROPERTIES
	# 	PASS_REGULAR_EXPRESSION "Saving simulation data")
	# set_tests_properties(${file_without_ext}
	# 	PROPERTIES
	# 	FAIL_REGULAR_EXPRESSION "(Exception|Test failed)")
	set_tests_properties(${file_without_ext}
		PROPERTIES
		TIMEOUT 40)
endforeach()
