include_directories("${SFML_INCLUDE_DIR}")

set(math_library "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")

add_executable(single single.cc physics.h constants.h)
target_link_libraries(single PUBLIC sfml-graphics sfml-window sfml-system ${math_library})

add_library(physics2 STATIC physics2.cc physics2.h)
target_link_libraries(physics2 PUBLIC sfml-graphics sfml-system ${math_library})

add_executable(motion2 main2.cc)
target_link_libraries(motion2 PUBLIC physics2 sfml-graphics sfml-window sfml-system)
add_custom_command(TARGET motion2 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:motion2>/FreeSans.ttf)

add_executable(single2 single2.cc)
target_link_libraries(single2 PUBLIC physics2 sfml-graphics sfml-window sfml-system)

add_executable(two2 two2.cc)
target_link_libraries(two2 PUBLIC physics2 sfml-graphics sfml-window sfml-system)
add_custom_command(TARGET two2 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/src/FreeSans.ttf $<TARGET_FILE_DIR:two2>/FreeSans.ttf)

# Tests
add_executable(test_physics2 EXCLUDE_FROM_ALL test_physics2.cc)
target_link_libraries(test_physics2 PUBLIC physics2)
