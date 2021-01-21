#include "physics2.h"

#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>

namespace {
	template <typename T>
	static sf::Vector2<T> center (const sf::Rect<T>& r) {
		return sf::Vector2<T>(r.left + r.width / 2, r.top + r.height / 2);
	}

	template <typename T>
	static sf::Rect<T> boundSquare (const sf::Vector2<T>& center, float r) {
		return sf::Rect<T>(center.x - r, center.y - r, r * 2, r * 2);
	}

	static inline float dot(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	static float distance(const sf::Vector2f& v1) {
		return std::hypot(v1.x, v1.y);
	}
}

namespace phys {
	sf::Vector2f PVector(float mag, float dir) {
		return sf::Vector2f(std::cos(dir * static_cast<float>(M_PI) / 180.f),
			std::sin(dir * static_cast<float>(M_PI) / 180.f)) * mag;
	}

	sf::CircleShape ParticleSystem::circle_shape(30);

	sf::Vector2f ParticleSystem::get_center(size_t index) {
		return sf::Vector2f(
			(vertices[index].position.x + vertices[index + 2].position.x) / 2,
			(vertices[index].position.y + vertices[index + 2].position.y) / 2);
	}

	void ParticleSystem::move_particle(size_t index, sf::Time elapsed) {
		sf::Vector2f displacement = particles[index].velocity
			* static_cast<float>(elapsed.asMilliseconds());
		for (size_t i = 0; i < 4; ++i) {
			vertices[index + i].position += displacement;
		}

		const sf::Vector2f &first = vertices[index].position, & third = vertices[index + 2].position;
		if (first.x < bounds.left || third.x >= bounds.left + bounds.width)
			particles[index].velocity.x *= -1.0f;
		if (first.y < bounds.top || third.y >= bounds.top + bounds.height)
			particles[index].velocity.y *= -1.0f;
		move_particle(index, elapsed);
	}

	void ParticleSystem::collide_particles(size_t i, size_t j) {
		sf::Vector2f v1, v2;
		sf::Vector2f pos1 = get_center(i), pos2 = get_center(j);
		Particle& alpha = particles[i], & beta = particles[j];
		v1 = alpha.velocity - dot(alpha.velocity - beta.velocity, pos1 - pos2)
			/ dot(pos1 - pos2, pos1 - pos2) * (pos1 - pos2);
		v2 = beta.velocity - dot(beta.velocity - alpha.velocity, pos2 - pos1)
			/ dot(pos2 - pos1, pos2 - pos1) * (pos2 - pos1);
		alpha.velocity = v1;
		beta.velocity = v2;
	}

	bool ParticleSystem::fast_infringe(size_t i, size_t j) {
#ifndef FAST_FAST
		sf::FloatRect r1(vertices[i].position, vertices[i + 2].position - vertices[i].position),
			r2(vertices[j].position, vertices[j + 2].position - vertices[j].position);
		return r1.intersects(r2);
#else
		sf::Vector2f& p1 = vertices[i].position, & p2 = vertices[j].position;
		return /* boolean expression */
#endif
	}

	bool ParticleSystem::slow_infringe(size_t i, size_t j) {
		// FIXME: It's possible the <= should be a <
		return distance(vertices[j].position - vertices[i].position)
			<= particles[i].radius + particles[j].radius;
	}

	void ParticleSystem::update(const sf::Time& elapsed) {
		for (size_t i = 0; i < particles.size(); ++i)
			move_particle(i, elapsed);
		for (size_t f = 0; f < particles.size() - 1; ++f)
			for (size_t o = f + 1; o < particles.size() - 1; ++o)
				if (fast_infringe(f, o) && slow_infringe(f, o))
					collide_particles(f, o);
	}

	size_t ParticleSystem::makeParticle(sf::Vector2f pos = {0, 0}, float r = 0.f, sf::Vector2f v = {0, 0}) {
		particles.emplace_back(v, r);
		// FIXME: Possibly set texCoords
		vertices.append(pos + sf::Vector2f(-r, -r));
		vertices.append(pos + sf::Vector2f(r, -r));
		vertices.append(pos + sf::Vector2f(r, r));
		vertices.append(pos + sf::Vector2f(-r, r));
		return particles.size() - 1;
	}

	size_t ParticleSystem::removeParticle(size_t n) {
		assert(particles.size() >= n);
		particles.resize(particles.size() - n);
		vertices.resize(vertices.getVertexCount() - n * 4);
		return particles.size();
	}
}
