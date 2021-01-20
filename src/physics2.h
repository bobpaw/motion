#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#ifndef MOTION_PHYSICS2_H_
#define MOTION_PHYSICS2_H_

namespace phys {
	class ParticleSystem: sf::Drawable, sf::Transformable {
		struct Particle {
			sf::Vector2f velocity;
			float radius;

			Particle(sf::Vector2f v, float r): velocity(v), radius(r) {}
		};
		std::vector<Particle> particles;
		sf::VertexArray vertices;
		sf::FloatRect bounds;
		ParticleSystem(float w, float h):
			bounds(0.f, 0.f, w, h),
			particles(),
			vertices(sf::Quads) {}
		void move_particle(sf::Time elapsed, size_t index);
		void collide_particles(size_t i, size_t j);
		void update(const sf::Time& elapsed);
		size_t makeParticle(sf::Vector2f pos = {0, 0}, float r = 0.f, sf::Vector2f v = {0, 0});
	private:

		sf::Vector2f get_center(size_t index);
		static sf::CircleShape circle_shape;
		bool fast_infringe(size_t i, size_t j);
		bool slow_infringe(size_t i, size_t j);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			// apply the entity's transform -- combine it with the one that was passed by the caller
			states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

			// apply the texture
			states.texture = circle_shape.getTexture();

			// you may also override states.shader or states.blendMode if you want

			// draw the vertex array
			target.draw(vertices, states);
		}
	};
}
#endif // MOTION_PHYSICS2_H_
