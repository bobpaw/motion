#include <iostream>
#include <random>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "physics2.h"

const int ScreenWidth = 640;
const int ScreenHeight = 400;

#undef main
int main (int argc, char* argv[]) {
	sf::RenderWindow graphics_window(sf::VideoMode(ScreenWidth, ScreenHeight), "Motion", sf::Style::Default /*, contenxt*/); // Window object
	graphics_window.setVerticalSyncEnabled(true);

	sf::Event event;
	sf::Clock clock;

	sf::Font freesans;
	freesans.loadFromFile("./FreeSans.ttf");

	sf::Text pressed("Pressed: False", freesans, 16);

	sf::Text location("(x, y): (NaN, NaN)", freesans, 16);
	location.setPosition(0, 16);

	phys::ParticleSystem screen(ScreenWidth, ScreenHeight, 10);

	screen.add_particle({140, 200}, phys::PVector(.1f, 0.0));
	screen.add_particle({500, 205}, phys::PVector(.1f, 180.0));
	screen.set_color(0, sf::Color::Red);

	sf::Vector2i startingSpot;
	sf::VertexArray arrow(sf::Lines, 2);
	arrow[0].color = sf::Color::Blue;
	
	sf::CircleShape ball(10);
	ball.setFillColor(sf::Color::White);
	ball.setOrigin(sf::Vector2f(10, 10));

	bool playing = false;

	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				// Check if X button (in top right) has been pressed
				graphics_window.close();
			} else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				startingSpot = sf::Mouse::getPosition(graphics_window);
				arrow[0].position = sf::Vector2f(startingSpot);
				pressed.setString("Pressed: True");
			} else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && startingSpot.x != -1 && startingSpot.y != -1) {
				screen.add_particle(sf::Vector2f(startingSpot), sf::Vector2f((sf::Mouse::getPosition(graphics_window) - startingSpot)) / 500.f);
				startingSpot = {-1, -1};
				pressed.setString("Pressed: False");
			} else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
				playing = !playing;
				clock.restart();
			}
		}

		// Motion
		if (playing) {
			screen.update(clock.restart());

			// Friction, etc.
		}


		// Clear screen
		graphics_window.clear();

		// Render sprites to screen
		graphics_window.draw(screen);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			arrow[1].position = sf::Vector2f(sf::Mouse::getPosition(graphics_window));
			ball.setPosition(arrow[0].position);
			ball.setFillColor(sf::Color::White);
			graphics_window.draw(ball);
			graphics_window.draw(arrow);
		} else {
			ball.setPosition(sf::Vector2f(sf::Mouse::getPosition(graphics_window)));
			ball.setFillColor(sf::Color::Transparent);
			ball.setOutlineThickness(1);
			graphics_window.draw(ball);
			ball.setOutlineThickness(0);
		}

		graphics_window.draw(pressed);
		location.setString("(x, y): (" + std::to_string(sf::Mouse::getPosition(graphics_window).x) + ", " + std::to_string(sf::Mouse::getPosition(graphics_window).y) + ")");
		graphics_window.draw(location);

		graphics_window.display();
	}
	return 0;
}
