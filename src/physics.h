#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/System.hpp>

#include "constants.h"

#ifndef MOTION_PHYSICS_H
#define MOTION_PHYSICS_H

namespace phys {
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

	// Where dt is in milliseconds
	struct PVector {
		double mag;
		double dir;

		PVector (double x = 0, double y = 0): mag(x), dir(y) {}

		double horizontal () const {
			return mag * std::cos(dir * M_PI / 180);
		}

		double vertical () const {
			return mag * std::sin(dir * M_PI / 180);
		}

		template <typename T>
		operator sf::Vector2<T> () {
			return sf::Vector2<T>(T(horizontal()), T(vertical()));
		}
	};

	struct Point2D {
		double x;
		double y;

		Point2D (double x_, double y_): x(x_), y(y_) {}

		void move (const PVector& velocity, const sf::Time &elapsed) {
			x += velocity.horizontal() * elapsed.asMilliseconds();
			y += velocity.vertical() * elapsed.asMilliseconds();
		}
	};

	struct Particle: public Point2D {
	public:
		PVector velocity;
		double mass;
		double radius;

		Particle (double x, double y, PVector v): Point2D(x, y), velocity(v), mass(0), radius(0) {}
		Particle (double x = 0.0, double y = 0.0): Particle(x, y, PVector(0.0, 0.0)) {}

		// [min, max)
		void move (sf::Time elapsed, int maxx, int maxy, int minx = 0, int miny = 0) {
			Point2D::move(velocity, elapsed);
			if ((x - radius) < minx || (x + radius) >= maxx) {
				velocity.dir = correct_deg(180 - velocity.dir);
				Point2D::move(velocity, elapsed);
			}
			if ((y - radius) < miny || (y + radius) >= maxy) {
				velocity.dir = correct_deg(velocity.dir * -1);
				Point2D::move(velocity, elapsed);
			}
		}
	};

	struct Plane {
	public:
		double height, width;
		Point2D center;

		std::vector<Particle> particles;
		Plane(double h, double w, Point2D c = Point2D(0, 0)): height(h), width(w), center(c), particles() {}
		size_t makeParticle (double x = 0, double y = 0, PVector v = PVector(0, 0)) {
			particles.emplace_back(x, y, v);
			return particles.size() - 1;
		}

		void moveParticle (size_t n, const sf::Time &elapsed) {
			particles[n].move(elapsed, static_cast<int>(width - center.x), static_cast<int>(height - center.y), static_cast<int>(-center.x), static_cast<int>(-center.y));
		}

		// Member particle access
		decltype(Particle::x)& x (size_t n) {
			assert(n < particles.size());
			return particles[n].x;
		}

		decltype(Particle::y)& y (size_t n) {
			assert(n < particles.size());
			return particles[n].y;
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
			for (auto& x : particles) x.move(elapsed, static_cast<int>(width - center.x), static_cast<int>(height - center.y), static_cast<int>(-center.x), static_cast<int>(-center.y));
		}
	};
} // namespace phys

#endif // MOTION_PHYSICS_H
