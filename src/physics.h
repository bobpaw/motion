#include <cmath>

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

	struct PVector {
		double mag;
		double dir;

		PVector (double x, double y): mag(x), dir(y) {}

		double horizontal () const {
			return mag * std::cos(dir * M_PI / 180);
		}

		double vertical () const {
			return mag * std::sin(dir * M_PI / 180);
		}
	};

	struct Point2D {
		double x;
		double y;

		Point2D (double x_, double y_): x(x_), y(y_) {}

		void move (const PVector& velocity) {
			x += velocity.horizontal();
			y += velocity.vertical();
		}
	};

	struct Plane {
		struct Particle: public Point2D {
		public:
			PVector velocity;
			double mass;
			double radius;

			Particle (double x, double y, PVector v): Point2D(x, y), velocity(v), mass(0), radius(0) {}
			Particle (double x = 0.0, double y = 0.0): Particle(x, y, PVector(0.0, 0.0)) {}

			// [min, max)
			void move (int maxx, int maxy, int minx = 0, int miny = 0) {
				Point2D::move(velocity);
				if ((x - radius) < minx || (x + radius) >= maxx) {
					velocity.dir = correct_deg(180 - velocity.dir);
					Point2D::move(velocity);
				}
				if ((y - radius) < miny || (y + radius) >= maxy) {
					velocity.dir = correct_deg(velocity.dir * -1);
					Point2D::move(velocity);
				}
			}
		};

		double height, width;
		std::vector<Particle> particles;
		Plane(double h, double w): height(h), width(w), particles() {}
		void makeParticle (double x, double y, PVector v) {
			particles.emplace_back(x, y, v);
		}

		void moveParticle (size_t n) {
			particles[n].move(static_cast<int>(width), static_cast<int>(height));
		}

		// Member particle access
		decltype(Particle::x)& x (size_t n) {
			return particles[n].x;
		}

		decltype(Particle::y)& y (size_t n) {
			return particles[n].y;
		}

		decltype(Particle::velocity)& velocity (size_t n) {
			return particles[n].velocity;
		}
	};
} // namespace phys

#endif // MOTION_PHYSICS_H
