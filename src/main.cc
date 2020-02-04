#include <iostream>
#include <random>
#include <cmath>

#include "SDL.h"


// Math constants from math.h (here so I don't need defines or whatever)
#define M_E            2.7182818284590452354   /* e */
#define M_LOG2E        1.4426950408889634074   /* log_2 e */
#define M_LOG10E       0.43429448190325182765  /* log_10 e */
#define M_LN2          0.69314718055994530942  /* log_e 2 */
#define M_LN10         2.30258509299404568402  /* log_e 10 */
#define M_PI           3.14159265358979323846  /* pi */
#define M_PI_2         1.57079632679489661923  /* pi/2 */
#define M_PI_4         0.78539816339744830962  /* pi/4 */
#define M_1_PI         0.31830988618379067154  /* 1/pi */
#define M_2_PI         0.63661977236758134308  /* 2/pi */
#define M_2_SQRTPI     1.12837916709551257390  /* 2/sqrt(pi) */
#define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
#define M_SQRT1_2      0.70710678118654752440  /* 1/sqrt(2) */

#define M_El           2.718281828459045235360287471352662498L /* e */
#define M_LOG2El       1.442695040888963407359924681001892137L /* log_2 e */
#define M_LOG10El      0.434294481903251827651128918916605082L /* log_10 e */
#define M_LN2l         0.693147180559945309417232121458176568L /* log_e 2 */
#define M_LN10l        2.302585092994045684017991454684364208L /* log_e 10 */
#define M_PIl          3.141592653589793238462643383279502884L /* pi */
#define M_PI_2l        1.570796326794896619231321691639751442L /* pi/2 */
#define M_PI_4l        0.785398163397448309615660845819875721L /* pi/4 */
#define M_1_PIl        0.318309886183790671537767526745028724L /* 1/pi */
#define M_2_PIl        0.636619772367581343075535053490057448L /* 2/pi */
#define M_2_SQRTPIl    1.128379167095512573896158903121545172L /* 2/sqrt(pi) */
#define M_SQRT2l       1.414213562373095048801688724209698079L /* sqrt(2) */
#define M_SQRT1_2l     0.707106781186547524400844362104849039L /* 1/sqrt(2) */

const int ScreenWidth = 640;
const int ScreenHeight = 400;

struct PVector {
	double mag;
	double dir;

	PVector (double x, double y): mag(x), dir(y) {}

	double horizontal () {
		return mag * std::cos(dir);
	}

	double vertical () {
		return mag * std::sin(dir);
	}
};

double correct_rad (double x) {
	while (x < 0) {
		x += 2 * M_PI;
	}
	while (x > 2 * M_PI) {
		x -= 2 * M_PI;
	}
	return x;
}

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
	int k_timeout_max = 1, k_timeout = 0;
	double x = 320, y = 200;
	PVector velocity(1, M_PI_2);
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
				velocity.mag += 0.02;
			} else if (!key_state[SDL_SCANCODE_UP] && key_state[SDL_SCANCODE_DOWN]) {
				velocity.mag -= 0.02;
			}
			if (key_state[SDL_SCANCODE_RIGHT] && !key_state[SDL_SCANCODE_LEFT]) {
				velocity.dir += 2 * M_PI / 180.0;
			} else if (!key_state[SDL_SCANCODE_RIGHT] && key_state[SDL_SCANCODE_LEFT]) {
				velocity.dir -= 2 * M_PI / 180.0;
			}
			if (key_state[SDL_SCANCODE_0]) {
				velocity.mag = 1;
				velocity.dir = M_PI_2;
			}
			k_timeout = k_timeout_max;
		}
		if (x + velocity.horizontal() >= ScreenWidth || x + velocity.horizontal() < 0) velocity.dir = correct_rad(M_PI - velocity.dir);
		if (y + velocity.vertical() >= ScreenHeight || y + velocity.vertical() < 0) velocity.dir = correct_rad(velocity.dir * -1.0);
		x += velocity.horizontal();
		y += velocity.vertical();
		SDL_SetRenderDrawColor(graphics_renderer, 0, 0, 0, 0);
		SDL_RenderClear(graphics_renderer); // Cover screen in a black rectangle, effectively clearing the screen
		// Render sprites to screen
		SDL_SetRenderDrawColor(graphics_renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(graphics_renderer, int(x), int(y));
		SDL_RenderDrawPoint(graphics_renderer, int(x) + 1, int(y));
		SDL_RenderDrawPoint(graphics_renderer, int(x), int(y) + 1);
		SDL_RenderDrawPoint(graphics_renderer, int(x) + 1, int(y) + 1);
		SDL_SetRenderDrawColor(graphics_renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(graphics_renderer, int(x), int(y), int(x + 10 * velocity.horizontal()), int(y + 10 * velocity.vertical()));
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
