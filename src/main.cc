#include <iostream>
#include <random>

#include "SDL.h"

#include "shapes.h"
#include "physics.h"

#include "constants.h"

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
	SDL_Texture* ball = SDL_CreateTexture(graphics_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 21, 21);
	int k_timeout_max = 1, k_timeout = 0;
	phys::Plane screen(ScreenHeight, ScreenWidth);
	screen.makeParticle(320, 200, phys::PVector(1, 0.0));
	screen.particles[0].radius = 10;
	SDL_SetRenderDrawColor(graphics_renderer, 255, 255, 255, 255);
	SDL_SetRenderTarget(graphics_renderer, ball);
	for (int i = 1; i < 11; ++i) DrawCircle(graphics_renderer, 10, 10, i);
	SDL_SetRenderTarget(graphics_renderer, NULL);
	SDL_Rect dest;
	dest.w = 21;
	dest.h = 21;
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
				screen.velocity(0).mag += 0.02;
			} else if (!key_state[SDL_SCANCODE_UP] && key_state[SDL_SCANCODE_DOWN]) {
				screen.velocity(0).mag -= 0.02;
			}
			if (key_state[SDL_SCANCODE_RIGHT] && !key_state[SDL_SCANCODE_LEFT]) {
				screen.velocity(0).dir += 2;
			} else if (!key_state[SDL_SCANCODE_RIGHT] && key_state[SDL_SCANCODE_LEFT]) {
				screen.velocity(0).dir -= 2;
			}
			if (key_state[SDL_SCANCODE_0]) {
				screen.velocity(0).mag = 1;
				screen.velocity(0).dir = 0;
			}
			k_timeout = k_timeout_max;
		}

		// Motion
		screen.moveParticle(0);

		// Clear screen
		SDL_SetRenderDrawColor(graphics_renderer, 0, 0, 0, 0);
		SDL_RenderClear(graphics_renderer); // Cover screen in a black rectangle, effectively clearing the screen
		// Render sprites to screen
		dest.x = static_cast<int>(screen.x(0) - screen.radius(0));
		dest.y = static_cast<int>(screen.y(0) - screen.radius(0));
		SDL_RenderCopy(graphics_renderer, ball, NULL, &dest);
		std::cout << "\rVelocity: " << screen.velocity(0).mag << "\tDirection: " << screen.velocity(0).dir << "     ";
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
