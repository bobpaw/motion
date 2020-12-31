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
	sf::RenderWindow graphics_window(sf::VideoMode(ScreenWidth, ScreenHeight), "Motion", sf::Style::Default /*, contenxt*/); // Window object
	sf::Event event;
	// const uint8_t* key_state = SDL_GetKeyboardState(nullptr); // Get address of keystate array and assign it to keyState pointer
	constexpr int ball_radius = 10;
	phys::Plane screen(ScreenHeight, ScreenWidth);
	
	screen.makeParticle(140, 200, phys::PVector(.1f, 0.0));
	screen.radius(0) = ball_radius;
	screen.makeParticle(500, 205, phys::PVector(.1f, 180.0));
	screen.radius(1) = ball_radius;

	sf::CircleShape ball(ball_radius);
	ball.setFillColor(sf::Color::White);
	// ball.transform something something turn on blending

	sf::Clock clock;
	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) { // SDL_PollEvent automatically updates key_state array
			if (event.type == sf::Event::Closed) {
				// Check if X button (in top right) has been pressed
				graphics_window.close();
			}
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

		graphics_window.display();
		sf::sleep(sf::milliseconds(1));
	}
	return 0;
}
