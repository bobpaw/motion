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
	private:
		double mag_;
		double dir_;
		double horiz_cache;
		double vert_cache;

		void update_cache () {
			horiz_cache = mag_ * std::cos(dir_ * M_PI / 180);
			vert_cache = mag_ * std::sin(dir_ * M_PI / 180);
		}

	public:

		PVector (double x = 0, double y = 0): mag_(x), dir_(y) { update_cache(); }

		auto mag() const noexcept { return mag_; }
		auto dir() const noexcept { return dir_; }

		decltype(mag_) set_mag(decltype(mag_) x) {
			mag_ = x;
			update_cache();
			return mag_;
		}

		decltype(dir_) set_dir(decltype(dir_) x) {
			dir_ = x;
			update_cache();
			return dir_;
		}

		decltype(mag_) move_mag(decltype(mag_) x) {
			mag_ += x;
			update_cache();
			return mag_;
		}

		decltype(dir_) move_dir(decltype(dir_) x) {
			dir_ += x;
			update_cache();
			return dir_;
		}

		decltype(horiz_cache) horizontal () const noexcept { return horiz_cache; }
		decltype(vert_cache) vertical () const noexcept { return vert_cache; }

		template <typename T>
		operator sf::Vector2<T> () const {
			return sf::Vector2<T>(T(horizontal()), T(vertical()));
		}
	};

	struct Point2D {
		double x;
		double y;

		Point2D (double x_, double y_): x(x_), y(y_) {}

		void move (const PVector& velocity, const sf::Time &elapsed) noexcept {
			x += velocity.horizontal() * elapsed.asMilliseconds();
			y += velocity.vertical() * elapsed.asMilliseconds();
		}

		double distance (const Point2D& other) const {
			return std::sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
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
				velocity.set_dir(correct_deg(180 - velocity.dir()));
				Point2D::move(velocity, elapsed);
			}
			if ((y - radius) < miny || (y + radius) >= maxy) {
				velocity.set_dir(correct_deg(velocity.dir() * -1));
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
