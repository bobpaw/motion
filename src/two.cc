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
	graphics_window.setVerticalSyncEnabled(true);
	sf::Event event;
	// const uint8_t* key_state = SDL_GetKeyboardState(nullptr); // Get address of keystate array and assign it to keyState pointer
	constexpr int ball_radius = 10;
	phys::Plane screen(ScreenHeight, ScreenWidth);
	
	screen.makeParticle(140, 200, ball_radius, phys::PVector(.1f, 0.0));
	screen.makeParticle(500, 205, ball_radius, phys::PVector(.1f, 180.0));
	sf::CircleShape ball(ball_radius);
	ball.setFillColor(sf::Color::White);
	ball.setOrigin(sf::Vector2f(ball_radius, ball_radius));
	// ball.transform something something turn on blending

	sf::Vector2i startingSpot;
	sf::VertexArray arrow(sf::Lines, 2);
	arrow[0].color = sf::Color::Blue;
	sf::Clock clock;

	sf::Font freesans;
	freesans.loadFromFile("./FreeSans.ttf");

	sf::Text pressed("Pressed: False", freesans);
	pressed.setCharacterSize(16);

	sf::Text location("(x, y): (NaN, NaN)", freesans, 16);
	location.setPosition(0, 16);

	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) { // SDL_PollEvent automatically updates key_state array
			if (event.type == sf::Event::Closed) {
				// Check if X button (in top right) has been pressed
				graphics_window.close();
			} else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				startingSpot = sf::Mouse::getPosition(graphics_window);
				arrow[0].position = phys::ConvertVector2<float>(startingSpot);
				pressed.setString("Pressed: True");
			} else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && startingSpot.x != -1 && startingSpot.y != -1) {
				screen.makeParticle(startingSpot.x, startingSpot.y, ball_radius, phys::ConvertVector2<float>((sf::Mouse::getPosition(graphics_window) - startingSpot)) / 500.f);
				startingSpot = {-1, -1};
				pressed.setString("Pressed: False");
			}
		}

		// Motion
		screen.moveAll(clock.restart());

		// Clear screen
		graphics_window.clear();

		// Render sprites to screen
		for (size_t i = 0; i < screen.pCount(); ++i) {
			ball.setPosition(screen.x(i), screen.y(i));
			graphics_window.draw(ball);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			arrow[1].position = phys::ConvertVector2<float>(sf::Mouse::getPosition(graphics_window));
			ball.setPosition(arrow[0].position);
			graphics_window.draw(ball);
			graphics_window.draw(arrow);
		}

		graphics_window.draw(pressed);
		location.setString("(x, y): (" + std::to_string(sf::Mouse::getPosition(graphics_window).x) + ", " + std::to_string(sf::Mouse::getPosition(graphics_window).y) + ")");
		graphics_window.draw(location);

		graphics_window.display();
		sf::sleep(sf::milliseconds(1));
	}
	return 0;
}
