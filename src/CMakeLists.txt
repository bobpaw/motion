set(SDL_NS "$<IF:$<BOOL:${PKG_CONFIG_FOUND}>,PkgConfig,SDL2>")

add_executable(motion main.cc physics.h constants.h shapes.h)
target_link_libraries(motion "${SDL_NS}::SDL2" "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")
# target_link_libraries(motion "$<if:VCPKG_FOUND,sfml-graphics sfml-window sfml-system,PkgConfig::SFML>")

add_executable(single single.cc physics.h constants.h shapes.h)
target_link_libraries(single "${SDL_NS}::SDL2" "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")

add_executable(demo_shapes demo_shapes.cc shapes.h)
target_link_libraries(demo_shapes "${SDL_NS}::SDL2" "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")
## target_compile_features(movinga PRIVATE ...)


# Tests
add_executable(test_physics EXCLUDE_FROM_ALL test_physics.cc physics.h)
target_compile_features(test_physics PRIVATE cxx_std_11)