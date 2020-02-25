#include <iostream>
#include <random>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "physics.h"
#include "constants.h"

const int ScreenWidth = 640;
const int ScreenHeight = 400;

#undef main
int main (int argc, char* argv[]) {
	std::random_device random;
	sf::RenderWindow graphics_window(sf::VideoMode(ScreenWidth, ScreenHeight), "Point Motion");
	sf::Event event;
	sf::CircleShape ball(10);
	int k_timeout_max = 1, k_timeout = 0;
	double max_speed = 1.3;
	phys::Plane screen(ScreenHeight, ScreenWidth);
	screen.makeParticle(320, 200, phys::PVector(0.004, 0.0));
	screen.particles[0].radius = 10;
	ball.setFillColor(sf::Color::White);

	sf::RenderTexture dial_texture;
	if (!dial_texture.create(31, 31)) return -1;
	dial_texture.clear();
	sf::CircleShape dial_circ(15);
	dial_circ.setFillColor(sf::Color::Black);
	dial_circ.setOutlineColor(sf::Color::White);
	dial_circ.setOutlineThickness(-1);
	dial_texture.draw(dial_circ);
	sf::VertexArray dial_line(sf::Lines, 2);
	dial_line[0].position = sf::Vector2f(15, 0);
	dial_line[1].position = sf::Vector2f(15, 10);
	dial_texture.draw(dial_line);

	dial_texture.display();
	sf::Sprite dial(dial_texture.getTexture());
	dial.setPosition(sf::Vector2f(15, ScreenHeight - 17));
	dial.setOrigin(sf::Vector2f(15, 15));

	sf::RectangleShape mag(sf::Vector2f(64, 20)), mag_inner(sf::Vector2f(30, 18));
	mag.setPosition(0, ScreenHeight - 52);
	mag_inner.setPosition(32, ScreenHeight - 51);
	mag.setFillColor(sf::Color::Black);
	mag.setOutlineColor(sf::Color::White);
	mag.setOutlineThickness(-1);
	mag_inner.setFillColor(sf::Color::White);

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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				screen.velocity(0).mag += 0.0002;
			} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				screen.velocity(0).mag -= 0.0002;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				screen.velocity(0).dir += 1;
			} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				screen.velocity(0).dir -= 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
				screen.velocity(0).mag = 1;
				screen.velocity(0).dir = 0.1;
			}
			k_timeout = k_timeout_max;
		}

		// Motion
		screen.moveParticle(0, clock.restart());

		// Clear screen
		graphics_window.clear();
		
		// Render sprites to screen
		ball.setPosition(static_cast<float>(screen.x(0) - screen.radius(0)), static_cast<float>(screen.y(0) - screen.radius(0)));
		dial.setRotation(static_cast<float>(screen.velocity(0).dir + 90));
		graphics_window.draw(ball);
		graphics_window.draw(dial);

		graphics_window.draw(mag);

		mag_inner.setScale(static_cast<float>(screen.velocity(0).mag / max_speed), 1);
		graphics_window.draw(mag_inner);
		std::cout << "\rVelocity: " << screen.velocity(0).mag << "\t\tDirection: " << screen.velocity(0).dir << "     ";
		graphics_window.display(); // Update screen based on changes
		// SDL_Delay(20); // Wait 20 milliseconds, should blip 50 fps
	}
	return 0;
}
