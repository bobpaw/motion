#include <iostream>
#include <random>
#include <vector>
#include <cmath>

#include "SDL.h"

#include "shapes.h"

#include "constants.h"

const int ScreenWidth = 640;
const int ScreenHeight = 400;

double correct_rad (double x) {
	while (x < 0) x += 2 * M_PI;
	while (x > 2 * M_PI) x -= 2 * M_PI;
	return x;
}

double correct_deg (double x) {
	while (x < 0.0) x += 360;
	while (x > 360.0) x -= 360.0;
	return x;
}

struct PVector {
	double mag;
	double dir;

	PVector (double x, double y): mag(x), dir(y) {}

	double horizontal () const {
		return mag * std::cos(dir * M_PI / 180);
	}

	double vertical () const {
		return mag * std::sin(dir * M_PI / 180);
	}
};

struct Point2D {
	double x;
	double y;
	
	Point2D (double x_, double y_): x(x_), y(y_) {}

	void move (const PVector &velocity) {
		x += velocity.horizontal();
		y += velocity.vertical();
	}
};

struct Plane {
	struct Particle: public Point2D {
	public:
		PVector velocity;
		double mass;
		double radius;

		Particle (double x, double y, PVector v): Point2D(x, y), velocity(v), mass(0), radius(0) {}
		Particle (double x = 0.0, double y = 0.0): Particle(x, y, PVector(0.0, 0.0)) {}

		// [min, max)
		void move (int maxx, int maxy, int minx = 0, int miny = 0) {
			Point2D::move(velocity);
			if ((x - radius) < minx || (x + radius) >= maxx) {
				velocity.dir = correct_deg(180 - velocity.dir);
				Point2D::move(velocity);
			}
			if ((y - radius) < miny || (y + radius) >= maxy) {
				velocity.dir = correct_deg(velocity.dir * -1);
				Point2D::move(velocity);
			}
		}
	};

	double height, width;
	std::vector<Particle> particles;
	Plane(double h, double w): height(h), width(w), particles() {}
	void makeParticle (double x, double y, PVector v) {
		particles.emplace_back(x, y, v);
	}

	void moveParticle (size_t n) {
		particles[n].move(static_cast<int>(width), static_cast<int>(height));
	}

	// Member particle access
	decltype(Particle::x)& x (size_t n) {
		return particles[n].x;
	}

	decltype(Particle::y)& y (size_t n) {
		return particles[n].y;
	}

	decltype(Particle::velocity)& velocity (size_t n) {
		return particles[n].velocity;
	}
};

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
	Plane screen(ScreenHeight, ScreenWidth);
	screen.makeParticle(320, 200, PVector(1, 45.0));
	screen.particles[0].radius = 10;
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
				screen.particles[0].velocity.mag += 0.02;
			} else if (!key_state[SDL_SCANCODE_UP] && key_state[SDL_SCANCODE_DOWN]) {
				screen.particles[0].velocity.mag -= 0.02;
			}
			if (key_state[SDL_SCANCODE_RIGHT] && !key_state[SDL_SCANCODE_LEFT]) {
				screen.particles[0].velocity.dir += 2;
			} else if (!key_state[SDL_SCANCODE_RIGHT] && key_state[SDL_SCANCODE_LEFT]) {
				screen.particles[0].velocity.dir -= 2;
			}
			if (key_state[SDL_SCANCODE_0]) {
				screen.particles[0].velocity.mag = 1;
				screen.particles[0].velocity.dir = 45;
			}
			k_timeout = k_timeout_max;
		}

		// Motion
		screen.moveParticle(0);

		// Clear screen
		SDL_SetRenderDrawColor(graphics_renderer, 0, 0, 0, 0);
		SDL_RenderClear(graphics_renderer); // Cover screen in a black rectangle, effectively clearing the screen
		// Render sprites to screen
		SDL_SetRenderDrawColor(graphics_renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(graphics_renderer, int(screen.particles[0].x), int(screen.particles[0].y));
		DrawCircle(graphics_renderer, screen.particles[0].x, screen.particles[0].y, screen.particles[0].radius);
		std::cout << "\rVelocity: " << screen.particles[0].velocity.mag << " Direction: " << screen.particles[0].velocity.dir;
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
