set(SDL_NS "$<IF:$<BOOL:${PKG_CONFIG_FOUND}>,PkgConfig,SDL2>")

add_executable(motion main.cc)
target_link_libraries(motion "${SDL_NS}::SDL2" "$<$<NOT:$<C_COMPILER_ID:MSVC>>:m>")
## target_compile_features(movinga PRIVATE ...)
