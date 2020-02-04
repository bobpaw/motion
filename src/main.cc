#include <iostream>
#include <random>

#include "SDL.h"

const int ScreenWidth = 640;
const int ScreenHeight = 400;

#undef main
int main (int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "SDL couldn't initialize! SDL_ERROR: " << SDL_GetError() << std::endl;
		return -1;
	}
	std::random_device random;
	SDL_Window* graphics_window = nullptr; // Window object
	SDL_Renderer* graphics_renderer = nullptr; // Surface of screen
	bool window_quit = false;
	SDL_Event event;
	const uint8_t* key_state = SDL_GetKeyboardState(nullptr); // Get address of keystate array and assign it to keyState pointer
	graphics_window = SDL_CreateWindow("Motion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
	// SDL_CreateWindow(name, windowx, windowy, width, height, options
	if (graphics_window == nullptr) {
		std::cerr << "SDL couldn't initialize window! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	graphics_renderer = SDL_CreateRenderer(graphics_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Use hardware acceleration and vsync
	int x = 320, y = 200, v_x = 1, v_y = 1, k_timeout_max = 20, k_timeout = 0;
	while (window_quit == false) {
		while (SDL_PollEvent(&event) != 0) { // SDL_PollEvent automatically updates key_state array
			if (event.type == SDL_QUIT) {
				// Check if X button (in top right) has been pressed
				window_quit = true;
			}
		}
		// Handle keyboard input
		if (k_timeout > 0) {
			--k_timeout;
		} else {
			if (key_state[SDL_SCANCODE_UP] && !key_state[SDL_SCANCODE_DOWN]) {
				--v_y;
			} else if (!key_state[SDL_SCANCODE_UP] && key_state[SDL_SCANCODE_DOWN]) {
				++v_y;
			}
			if (key_state[SDL_SCANCODE_RIGHT] && !key_state[SDL_SCANCODE_LEFT]) {
				++v_x;
			} else if (!key_state[SDL_SCANCODE_RIGHT] && key_state[SDL_SCANCODE_LEFT]) {
				--v_x;
			}
			if (key_state[SDL_SCANCODE_0]) {
				v_x = 1;
				v_y = 1;
			}
			k_timeout = k_timeout_max;
		}
		if (x + v_x >= ScreenWidth || x + v_x < 0) v_x = -v_x;
		if (y + v_y >= ScreenHeight || y + v_y < 0) v_y = -v_y;
		x += v_x;
		y += v_y;
		SDL_SetRenderDrawColor(graphics_renderer, 0, 0, 0, 0);
		SDL_RenderClear(graphics_renderer); // Cover screen in a black rectangle, effectively clearing the screen
		// Render sprites to screen
		SDL_SetRenderDrawColor(graphics_renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(graphics_renderer, x, y);
		SDL_RenderDrawPoint(graphics_renderer, x + 1, y);
		SDL_RenderDrawPoint(graphics_renderer, x, y + 1);
		SDL_RenderDrawPoint(graphics_renderer, x + 1, y + 1);
		SDL_RenderPresent(graphics_renderer); // Update screen based on changes
		// SDL_Delay(20); // Wait 20 milliseconds, should blip 50 fps
	}
	SDL_RenderClear(graphics_renderer);
	SDL_DestroyRenderer(graphics_renderer);
	SDL_DestroyWindow(graphics_window); // Destroy window; should free surface associated with screen.
	graphics_window = nullptr;
	graphics_renderer = nullptr;
	SDL_Quit(); // Quit and unload SDL module
	return 0;
}
