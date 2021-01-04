#include <iostream>
#include <random>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "physics.h"

#include "constants.h"

constexpr int ScreenWidth = 640;
constexpr int ScreenHeight = 400;
constexpr int ball_radius = 5;

#undef main
int main (int argc, char* argv[]) {
	std::mt19937 random_engine(std::random_device{}());
	std::uniform_real_distribution<float> widthR(ball_radius, ScreenWidth - ball_radius),
		heightR(ball_radius, ScreenHeight - ball_radius), degreeR(0, 360);

	sf::RenderWindow graphics_window(sf::VideoMode(ScreenWidth, ScreenHeight), "Motion", sf::Style::Default, sf::ContextSettings(0, 0, 8));
	graphics_window.setVerticalSyncEnabled(true);

	// Our one ball shape
	sf::CircleShape ball(ball_radius);

	sf::Clock clock;
	sf::Event event;
	
	// Texty
	sf::Font freesans;
	freesans.loadFromFile("./FreeSans.ttf");

	sf::Text pCount("Particles: 1", freesans, 16);

	// Awkward timers so holding keys down doesn't get handled too fast; probably do with an sf::Clock eventually
	int k_timeout_max = 1, k_timeout_max_min = 1, k_timeout_max_max = 4, k_timeout = 0;
	
	phys::Plane screen(ScreenHeight, ScreenWidth);
	screen.makeParticle(320, 200, ball_radius, phys::PVector(.1f, 0.0));

	clock.restart();
	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				graphics_window.close();
			}
		}

		// Handle keys that can be held down
		if (k_timeout > 0) {
			--k_timeout;
		} else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Dash) && screen.particles.size() < 200) {
				// The random engine doesn't work correctly if they're inline calls in makeParticle
				float x = widthR(random_engine), y = heightR(random_engine), dir = degreeR(random_engine);
				screen.makeParticle(x, y, ball_radius, phys::PVector(.25f, dir));
				k_timeout_max -= (k_timeout_max > k_timeout_max_min ? 1 : 0);
				pCount.setString("Particles: " + std::to_string(screen.pCount()));
			} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Equal) && sf::Keyboard::isKeyPressed(sf::Keyboard::Dash) && screen.particles.size() > 1) {
				screen.particles.pop_back();
				pCount.setString("Particles: " + std::to_string(screen.pCount()));
			} else {
				k_timeout_max = k_timeout_max_max;
			}
			k_timeout = k_timeout_max;
		}

		// Motion
		screen.moveAll(clock.restart());

		// Clear screen
		graphics_window.clear();
		
		graphics_window.draw(pCount);

		// Render sprites to screen
		for (size_t i = 0; i < screen.pCount(); ++i) {
			ball.setPosition(screen.x(i) - screen.radius(i), screen.y(i) - screen.radius(i));
			graphics_window.draw(ball);
		}
		graphics_window.display();
		sf::sleep(sf::milliseconds(10)); // A number high enough that CPU doesn't overtake graphics output
	}
	return 0;
}
