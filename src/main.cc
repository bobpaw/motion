#include <iostream>
#include <random>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "physics.h"

#include "constants.h"

const int ScreenWidth = 640;
const int ScreenHeight = 400;

	#undef main
int main (int argc, char* argv[]) {
	std::random_device random;
	sf::RenderWindow graphics_window(sf::VideoMode(ScreenWidth, ScreenHeight), "Motion", sf::Style::Default /*, contenxt*/ ); // Window object
	sf::Event event;
	// const uint8_t* key_state = SDL_GetKeyboardState(nullptr); // Get address of keystate array and assign it to keyState pointer
	constexpr int ball_radius = 10;
	int k_timeout_max = 1, k_timeout_max_min = 1, k_timeout_max_max = 20, k_timeout = 0;
	phys::Plane screen(ScreenHeight, ScreenWidth);
	screen.makeParticle(320, 200, phys::PVector(1, 0.0));
	screen.radius(0) = ball_radius;
	sf::CircleShape ball(ball_radius);
	ball.setFillColor(sf::Color::White);
	// ball.transform something something turn on blending
	std::uniform_int_distribution<> widthR(ball_radius, ScreenWidth - ball_radius), heightR(ball_radius, ScreenHeight - ball_radius), degreeR(0, 360);
	sf::Clock clock;
	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) { // SDL_PollEvent automatically updates key_state array
			if (event.type == sf::Event::Closed) {
				// Check if X button (in top right) has been pressed
				graphics_window.close();
			}
		}
		// Handle keyboard input
		if (k_timeout > 0) {
			--k_timeout;
		} else {
			if (0 /* && key_state[SDL_SCANCODE_EQUALS] && !key_state[SDL_SCANCODE_MINUS] */) {
				screen.makeParticle(widthR(random), heightR(random), phys::PVector(1, degreeR(random)));
				screen.radius(screen.pCount() - 1) = ball_radius;
				k_timeout_max -= (k_timeout_max > k_timeout_max_min ? 1 : 0);
			} else {
				k_timeout_max = k_timeout_max_max;
			}
			k_timeout = k_timeout_max;
		}

		// Motion
		screen.moveAll(clock.restart());

		// Clear screen
		graphics_window.clear();
			
		// Render sprites to screen
		for (size_t i = 0; i < screen.pCount(); ++i) {
			ball.setPosition(screen.x(i) - screen.radius(i), screen.y(i) - screen.radius(i));
			graphics_window.draw(ball);
		}
		graphics_window.display(); // Update screen based on changes
		// SDL_Delay(20); // Wait 20 milliseconds, should blip 50 fps
	}
	return 0;
}
