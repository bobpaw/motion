include_directories("${SFML_INCLUDE_DIR}")

set(math_library "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")

add_executable(motion main.cc physics.h constants.h)
target_link_libraries(motion PUBLIC sfml-graphics sfml-window sfml-system ${math_library})
add_custom_command(TARGET motion PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:motion>/FreeSans.ttf)

add_executable(single single.cc physics.h constants.h)
target_link_libraries(single PUBLIC sfml-graphics sfml-window sfml-system ${math_library})

add_executable(two two.cc physics.h constants.h)
target_link_libraries(two PUBLIC sfml-graphics sfml-window sfml-system ${math_library})

add_library(physics2 STATIC physics2.cc physics2.h)
target_link_libraries(physics2 PUBLIC sfml-graphics sfml-system ${math_library})

add_executable(motion2 main2.cc)
target_link_libraries(motion2 PUBLIC physics2 sfml-graphics sfml-window sfml-system)
add_custom_command(TARGET motion2 PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:motion2>/FreeSans.ttf)

add_executable(two2 two2.cc)
target_link_libraries(two2 PUBLIC physics2 sfml-graphics sfml-window sfml-system)

# Tests
add_executable(test_physics EXCLUDE_FROM_ALL test_physics.cc physics.h)
target_compile_features(test_physics PRIVATE cxx_std_11)
target_link_libraries(test_physics PUBLIC ${math_library})

add_executable(test_physics2 EXCLUDE_FROM_ALL test_physics2.cc)
target_link_libraries(test_physics2 PUBLIC physics2)
