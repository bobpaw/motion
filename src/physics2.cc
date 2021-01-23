#include "physics2.h"

#include <cassert>
#include <cmath>

#include "constants.h"

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

	sf::RenderTexture ParticleSystem::circle_shape;
	ParticleSystem::StaticConstructor::StaticConstructor () {
		circle_shape.create(61, 61);
		sf::CircleShape shape(30);
		circle_shape.draw(shape);
		circle_shape.display();
	}

	ParticleSystem::StaticConstructor ParticleSystem::_StaticConstructor;

	sf::Vector2f ParticleSystem::get_center(size_t index) const {
		return sf::Vector2f(
			(vertices[index * 4].position.x + vertices[index * 4 + 2].position.x) / 2,
			(vertices[index * 4].position.y + vertices[index * 4 + 2].position.y) / 2);
	}

	void ParticleSystem::move_particle(size_t index, sf::Vector2f displacement) {
		for (size_t i = 0; i < 4; ++i) {
			vertices[index * 4 + i].position += displacement;
		}
	}

	void ParticleSystem::move_particle(size_t index, sf::Time elapsed) {
		sf::Vector2f displacement = particles[index].velocity
			* static_cast<float>(elapsed.asMilliseconds());
		
		move_particle(index, displacement);

		const sf::Vector2f& first = vertices[index * 4].position, & third = vertices[index * 4 + 2].position;
		bool redo = false;
		if (first.x < bounds.left || third.x >= bounds.left + bounds.width) {
			particles[index].velocity.x *= -1.0f;
			displacement.x *= -1.0f;
			redo = true;
		}
		if (first.y < bounds.top || third.y >= bounds.top + bounds.height) {
			particles[index].velocity.y *= -1.0f;
			displacement.y *= -1.0f;
			redo = true;
		}

		if (redo) move_particle(index, displacement);
	}

	void ParticleSystem::collide_particles(size_t i, size_t j) {
		sf::Vector2f pos1 = get_center(i), pos2 = get_center(j);
		sf::Vector2f v1 = particles[i].velocity, v2 = particles[j].velocity;
		particles[i].velocity = v1 - dot(v1 - v2, pos1 - pos2)
			/ dot(pos1 - pos2, pos1 - pos2) * (pos1 - pos2);
		particles[j].velocity = v2 - dot(v2 - v1, pos2 - pos1)
			/ dot(pos2 - pos1, pos2 - pos1) * (pos2 - pos1);
	}

	bool ParticleSystem::fast_infringe(size_t i, size_t j) const {
#ifndef FAST_FAST
		i *= 4; j *= 4;
		sf::FloatRect r1(vertices[i].position, vertices[i + 2].position - vertices[i].position),
			r2(vertices[j].position, vertices[j + 2].position - vertices[j].position);
		return r1.intersects(r2);
#else
		sf::Vector2f& p1 = vertices[i].position, & p2 = vertices[j].position;
		return /* boolean expression */
#endif
	}

	bool ParticleSystem::slow_infringe(size_t i, size_t j) const {
		// FIXME: It's possible the <= should be a <
		return distance(vertices[j * 4].position - vertices[i * 4].position)
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

	size_t ParticleSystem::add_particle(sf::Vector2f pos, sf::Vector2f v, float r) {
		if (r <= 0) r = default_radius_;
		particles.emplace_back(v, r);
		// FIXME: Possibly set texCoords
		vertices.append({pos + sf::Vector2f(-r, -r), sf::Vector2f(0.f, 0.f)});
		vertices.append({pos + sf::Vector2f(r, -r), sf::Vector2f(61.f, 0.f)});
		vertices.append({pos + sf::Vector2f(r, r), sf::Vector2f(61.f, 61.f)});
		vertices.append({pos + sf::Vector2f(-r, r), sf::Vector2f(0.f, 61.f)});

		return particles.size() - 1;
	}

	size_t ParticleSystem::remove_particle(size_t n) {
		assert(particles.size() >= n);
		particles.resize(particles.size() - n);
		vertices.resize(vertices.getVertexCount() - n * 4);
		return particles.size();
	}
}
