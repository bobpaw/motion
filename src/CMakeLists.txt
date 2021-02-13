include_directories("${SFML_INCLUDE_DIR}")

set(math_library "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")

add_library(physics STATIC physics.cc physics.h)
target_link_libraries(physics PUBLIC sfml-graphics sfml-system ${math_library})

add_executable(motion main.cc)
target_link_libraries(motion PUBLIC physics sfml-graphics sfml-window sfml-system)
add_custom_command(TARGET motion POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:motion>/FreeSans.ttf)

add_executable(single single.cc)
target_link_libraries(single PUBLIC physics sfml-graphics sfml-window sfml-system)

add_executable(two two.cc)
target_link_libraries(two PUBLIC physics sfml-graphics sfml-window sfml-system)
add_custom_command(TARGET two POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:two>/FreeSans.ttf)

# Tests
add_executable(test_physics EXCLUDE_FROM_ALL test_physics.cc)
target_link_libraries(test_physics PUBLIC physics)
