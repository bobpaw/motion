#ifndef MOTION_SDL_CALL_H
#define MOTION_SDL_CALL_H

#define SDL_STR(x) #x
#define SDL_ERROR2(func) std::cerr << "SDL_" #func ": " << SDL_GetError() << std::endl
#ifndef NDEBUG
#define SDL_ERROR(func) do { \
std::cerr << __FILE__ "+" SDL_STR(__LINE__) ":" << std::endl; \
SDL_ERROR2(func); \
} while (0)
#else
#define SDL_ERROR(func) SDL_ERROR2(func)
#endif

#define SDL_CONCAT2(x, y) x##y
#define SDL_CONCAT(x, y) SDL_CONCAT2(x, y)

#define SDL_CALL(func, ...) do { \
if (SDL_CONCAT(SDL_, func)(__VA_ARGS__) < 0) { \
SDL_ERROR(func); \
SDL_QuitSubSystem(SDL_INIT_EVERYTHING); \
SDL_Quit(); \
return -1; \
} \
} while(0)

#endif // MOTION_SDL_CALL_H