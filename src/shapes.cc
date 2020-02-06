#include <iostream>
#include <cmath>

#include <SDL.h>

#include "shapes.h"

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


#undef main
int main (int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "SDL couldn't initialize! SDL_ERROR: " << SDL_GetError() << std::endl;
		SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
		SDL_Quit();
		return -1;
	}
	SDL_Window* graphics_window = nullptr; // Window object
	SDL_Renderer* graphics_renderer = nullptr; // Surface of screen
	bool window_quit = false;
	SDL_Event event;
	graphics_window = SDL_CreateWindow("Shapes", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 400, SDL_WINDOW_SHOWN);
	// SDL_CreateWindow(name, windowx, windowy, width, height, options
	if (graphics_window == nullptr) {
		std::cerr << "SDL couldn't initialize window! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
		SDL_Quit();
		return -1;
	}
	graphics_renderer = SDL_CreateRenderer(graphics_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_CALL(SetRenderDrawColor, graphics_renderer, 0, 0, 0, 0);
	SDL_CALL(RenderClear, graphics_renderer);
	// Render sprites to screen
	SDL_CALL(SetRenderDrawColor, graphics_renderer, 255, 255, 255, 255);
	SDL_CALL(RenderDrawPoint, graphics_renderer, 20, 30);
	DrawCircle(graphics_renderer, 20, 30, 15);
	SDL_CALL(RenderDrawPoint, graphics_renderer, 55, 30);
	for (int i = 10; i < 16; ++i)
	DrawCircle(graphics_renderer, 55, 30, i, true);
	SDL_RenderPresent(graphics_renderer); // Returns void
	// SDL_Delay(20); Using VSync
	do {
		if (SDL_WaitEvent(&event) == 0) {
			SDL_ERROR(SDL_WaitEvent);
			break;
		}
	} while (event.type != SDL_QUIT);
	SDL_CALL(RenderClear, graphics_renderer);
	SDL_DestroyRenderer(graphics_renderer);
	SDL_DestroyWindow(graphics_window);
	graphics_window = nullptr;
	graphics_renderer = nullptr;
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
	return 0;
}
