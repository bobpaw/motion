add_test(
	NAME
		Physics2Test
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target test_physics2
			--test-command "$<TARGET_FILE:test_physics2>"
)
