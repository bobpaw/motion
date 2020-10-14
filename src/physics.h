#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/System.hpp>

#include "constants.h"

#ifndef MOTION_PHYSICS_H
#define MOTION_PHYSICS_H

namespace phys {

	template <typename T>
	std::enable_if_t<std::is_floating_point_v<T>,typename T> correct_rad (T x) {
		while (x < 0) x += static_cast<T>(2 * M_PI);
		while (x > static_cast<T>(2 * M_PI)) x -= static_cast<T>(2 * M_PI);
		return x;
	}

	template <typename T>
	std::enable_if_t<std::is_arithmetic<T>::value, typename T> correct_deg (T x) {
		while (x < 0.0) x += static_cast<T>(360);
		while (x > 360.0) x -= static_cast<T>(360);
		return x;
	}

	sf::Vector2f&& PVector(float mag, float dir) {
		sf::Vector2f ret(mag * std::cos(dir * static_cast<float>(M_PI) / 180.f), mag * std::sin(dir * static_cast<float>(M_PI) / 180.f));
		return std::move(ret);
	}

	sf::Vector2f PVector(const sf::Vector2f& v) {
		return sf::Vector2f(std::hypot(v.x, v.y), std::atan2(v.y, v.x) * 180.0f / static_cast<float>(M_PI));
	}

	struct Point2D {
		float x;
		float y;

		Point2D (float x_, float y_): x(x_), y(y_) {}

		void move (const sf::Vector2f& velocity, const sf::Time &elapsed) noexcept {
			x += velocity.x * elapsed.asMilliseconds();
			y += velocity.y * elapsed.asMilliseconds();
		}

		float distance (const Point2D& other) const {
			return std::hypot(other.x - x, other.y - y);
		}
	};

	float dot (const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	float distance (const sf::Vector2f& v1) {
		return std::hypot(v1.x, v1.y);
	}

	struct Particle {
	public:
		sf::Vector2f velocity;
		sf::Vector2f pos;
		float mass;
		float radius;

		Particle (float x, float y, sf::Vector2f v): pos(x, y), velocity(v), mass(0), radius(0) {}
		Particle (float x = 0.0f, float y = 0.0f): Particle(x, y, sf::Vector2f(0.0, 0.0)) {}

		// [min, max)
		void move (sf::Time elapsed, int maxx, int maxy, int minx = 0, int miny = 0) {
			// sf::Vector2f projected_pos = pos + velocity * static_cast<float>(elapsed.asMilliseconds())
			pos += velocity * static_cast<float>(elapsed.asMilliseconds());
			if ((pos.x - radius) < minx || (pos.x + radius) >= maxx) {
				velocity.x *= -1.0f;
				pos += velocity * static_cast<float>(elapsed.asMilliseconds());
			}
			if ((pos.y - radius) < miny || (pos.y + radius) >= maxy) {
				velocity.y *= -1.0f;
				pos += velocity * static_cast<float>(elapsed.asMilliseconds());
			}
		}

		// Changes both particles
		void collide (Particle& other) {
			/*
			sf::Vector2f v1, v2;
			v1 = velocity - dot(velocity - other.velocity, pos - other.pos) / distance(pos - other.pos) * (pos - other.pos);
			v2 = other.velocity - dot(other.velocity - velocity, other.pos - pos) / distance(other.pos - pos) * (other.pos - pos);
			velocity = v1;
			other.velocity = v2;
			*/
			std::swap(velocity, other.velocity);
		}
	};

	struct Plane {
	public:
		unsigned int height, width;
		Point2D center;

		std::vector<Particle> particles;
		Plane(unsigned int h, unsigned int w, Point2D c = Point2D(0, 0)): height(h), width(w), center(c), particles() {}
		size_t makeParticle (float x = 0, float y = 0, sf::Vector2f v = sf::Vector2f(0, 0)) {
			particles.emplace_back(x, y, v);
			return particles.size() - 1;
		}

		void moveParticle (size_t n, const sf::Time &elapsed) {
			particles[n].move(elapsed, static_cast<int>(width - center.x), static_cast<int>(height - center.y), static_cast<int>(-center.x), static_cast<int>(-center.y));
		}

		// Member particle access
		float& x (size_t n) {
			assert(n < particles.size());
			return particles[n].pos.x;
		}

		float& y (size_t n) {
			assert(n < particles.size());
			return particles[n].pos.y;
		}

		decltype(Particle::velocity)& velocity (size_t n) {
			assert(n < particles.size());
			return particles[n].velocity;
		}

		decltype(Particle::radius)& radius (size_t n) {
			assert(n < particles.size());
			return particles[n].radius;
		}

		decltype(particles.size()) pCount () const { return particles.size(); }

		void moveAll (const sf::Time &elapsed) {
			for (size_t i = 0; i < particles.size(); ++i) {
				moveParticle(i, elapsed);
			}
			/*for (size_t f = 0; f < particles.size(); ++f) {
				for (size_t o = 0; o < particles.size(); ++o) {
					if (particles[f].distance(particles[o]) <= radius(f) + radius(o)) {
						// collide;
					}
				}
			}*/
		}
	};
} // namespace phys

#endif // MOTION_PHYSICS_H
