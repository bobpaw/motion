add_test(
	NAME
		PhysicsTest
	COMMAND
		"${CMAKE_CTEST_COMMAND}"
			--build-and-test "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
			--build-generator "${CMAKE_GENERATOR}"
			--build-nocmake
			--build-target test_physics
			--test-command "$<TARGET_FILE:test_physics>"
)
