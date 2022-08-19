#include <vector>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#ifndef ENT_MOOT_H_
#define ENT_MOOT_H_

namespace moot {
	class EntityBox : public sf::Drawable, public sf::Transformable {
		enum EntityType : bool { Sprite = false, Hitbox = true};
		struct Entity {
			bool HitBox;
			sf::FloatRect pos;
			sf::FloatRect texCoords;
			sf::Color color;

			Entity(bool, FloatRect);
			Entity(bool, FloatRect, FloatRect);
			Entity(bool, FloatRect, color);
			Entity(bool, FloatRect, FloatRect, color);
		};
		
		

		sf::Texture* tex;

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
		EntityList::HitBoxIterator begin();
		EntityList::SpriteIterator frontSprite();
		EntityBox(sf::Texture& tex);
	};
} // namespace moot

#endif // ENT_MOOT_H_
