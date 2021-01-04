include_directories("${SFML_INCLUDE_DIR}")
add_executable(motion main.cc physics.h constants.h)
target_link_libraries(motion PUBLIC sfml-graphics sfml-window sfml-system $<$<NOT:$<C_COMPILER_ID:MSVC>>:m>)
add_custom_command(TARGET motion PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:motion>/FreeSans.ttf)

add_executable(single single.cc physics.h constants.h)
target_link_libraries(single PUBLIC sfml-graphics sfml-window sfml-system $<$<NOT:$<C_COMPILER_ID:MSVC>>:m>)

add_executable(two two.cc physics.h constants.h)
target_link_libraries(two PUBLIC sfml-graphics sfml-window sfml-system $<$<NOT:$<C_COMPILER_ID:MSVC>>:m>)

# Tests
add_executable(test_physics EXCLUDE_FROM_ALL test_physics.cc physics.h)
target_compile_features(test_physics PRIVATE cxx_std_11)
target_link_libraries(test_physics PUBLIC_ $<$<NOT:$<C_COMPILER_ID:MSVC>>:m>)