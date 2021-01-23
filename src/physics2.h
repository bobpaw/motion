#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#ifndef MOTION_PHYSICS2_H_
#define MOTION_PHYSICS2_H_

namespace phys {
	sf::Vector2f PVector(float mag, float dir);

	class ParticleSystem: public sf::Drawable, public sf::Transformable {
	protected:
		struct Particle {
			sf::Vector2f velocity;
			float radius;

			Particle(sf::Vector2f v = {0, 0}, float r = 1): velocity(v), radius(r) {}
		};
		std::vector<Particle> particles;
		sf::VertexArray vertices;
		sf::FloatRect bounds;
		float default_radius_;
		
		void move_particle(size_t index, sf::Time elapsed);

		// Helper functions for collide_particles
		bool fast_infringe(size_t i, size_t j) const;
		bool slow_infringe(size_t i, size_t j) const;

		void collide_particles(size_t i, size_t j);

		static sf::RenderTexture circle_shape;
		static struct StaticConstructor {
			StaticConstructor ();
		} _StaticConstructor;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			// apply the entity's transform -- combine it with the one that was passed by the caller
			states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

			// apply the texture
			states.texture = &circle_shape.getTexture();

			// you may also override states.shader or states.blendMode if you want

			// draw the vertex array
			target.draw(vertices, states);
		}
	public:
		ParticleSystem(float w, float h, float r = 1.f): default_radius_(r),
			bounds(0.f, 0.f, w, h), particles(), vertices(sf::Quads) {
			assert(default_radius_ > 0.f);
		}

		void update(const sf::Time& elapsed);
		size_t add_particle(sf::Vector2f pos = {0, 0}, sf::Vector2f v = {0, 0}, float r = -1.f);
		size_t remove_particle(size_t n = 1);

		const float& default_radius() const noexcept {
			return default_radius_;
		}

		float& default_radius() noexcept {
			return default_radius_;
		}

		size_t pCount() const {
			return particles.size();
		}

		sf::Vector2f get_center(size_t index) const;
		void move_particle(size_t index, sf::Vector2f displacement);
	};
}
#endif // MOTION_PHYSICS2_H_
