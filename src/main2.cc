#include <iostream>
#include <random>
#include <filesystem>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "physics2.h"

namespace fs = std::filesystem;

constexpr int ScreenWidth = 640;
constexpr int ScreenHeight = 400;
constexpr int ball_radius = 5;

#undef main
int main (int argc, char* argv[]) {
	std::mt19937 random_engine(std::random_device{}());
	std::uniform_real_distribution<float> widthR(ball_radius, ScreenWidth - ball_radius),
		heightR(ball_radius, ScreenHeight - ball_radius), degreeR(0, 360);

	sf::RenderWindow graphics_window(sf::VideoMode(ScreenWidth, ScreenHeight), "Motion", sf::Style::Default, sf::ContextSettings(0, 0, 0));
	graphics_window.setVerticalSyncEnabled(true);

	sf::Clock clock, keyclock;
	sf::Event event;

	// Texty stuff
	sf::Font freesans;
	freesans.loadFromFile((fs::path(argv[0]).parent_path() / "FreeSans.ttf").string());

	sf::Text pCount("Particles: 1", freesans, 16);

	phys::ParticleSystem screen(ScreenWidth, ScreenHeight, 5);
	screen.add_particle({320, 200}, phys::PVector(.1f, 0.0));
	
	int held_times = 0;

	// Some subroutines that'll be members when the app becomes a class
	auto add_particle = [&]() {
		if (screen.pCount() < 200) {
			float x = widthR(random_engine), y = heightR(random_engine), dir = degreeR(random_engine);
			screen.add_particle({x, y}, phys::PVector(.25f, dir));
			pCount.setString("Particles: " + std::to_string(screen.pCount()) + " H ct: " + std::to_string(held_times));
		}
	};

	auto remove_particle = [&]() {
		if (screen.pCount() > 1) {
			screen.remove_particle();
			pCount.setString("Particles: " + std::to_string(screen.pCount()));
		}
	};

	clock.restart();
	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				graphics_window.close();
			} else if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code) {
					case sf::Keyboard::Equal:
						add_particle();
						break;
					case sf::Keyboard::Dash:
						remove_particle();
						break;
				}
			}
		}

		// Handle keys that can be held down

		/*
		if (keyclock.getElapsedTime().asMilliseconds() >= std::max(400 - held_times * 20, 100)) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
				add_particle();
			} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Equal) && sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
				remove_particle();
			}
			++held_times;
			keyclock.restart();
		} */

		// Motion
		screen.update(clock.restart());

		// Clear screen
		graphics_window.clear();

		graphics_window.draw(pCount);

		// Render sprites to screen
		graphics_window.draw(screen);

		graphics_window.display();
		// sf::sleep(sf::milliseconds(10)); // A number high enough that CPU doesn't overtake graphics output
	}
	return 0;
}
