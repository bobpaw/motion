set(SDL_NS "$<IF:$<BOOL:${PKG_CONFIG_FOUND}>,PkgConfig,SDL2>")

include_directories("${SFML_INCLUDE_DIR}")
add_executable(motion main.cc physics.h constants.h)
# target_link_libraries(motion PUBLIC "${SDL_NS}::SDL2" "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")
target_link_libraries(motion PUBLIC sfml-graphics sfml-window sfml-system)

add_executable(single single.cc physics.h constants.h)
target_link_libraries(single PUBLIC sfml-graphics sfml-window sfml-system)

# Tests
add_executable(test_physics EXCLUDE_FROM_ALL test_physics.cc physics.h)
target_compile_features(test_physics PRIVATE cxx_std_11)