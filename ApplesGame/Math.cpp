#include "Math.h"
#include <vector>
#include "Actor.h"

namespace ApplesGame
{
	sf::Vector2f OurVectorToSf(const Vector2D& v)
	{
		return sf::Vector2f(v.x, v.y);
	}

	sf::Vector2f GetScale(const sf::Shape& shape, const Vector2D& desiredSize)
	{
		const sf::Vector2u textureSize = shape.getTexture()->getSize();
		const sf::Vector2f shapeScale = { desiredSize.x / textureSize.x, desiredSize.y / textureSize.y };
		return shapeScale;
	}

	sf::Vector2f GetSpriteScale(const sf::Sprite& sprite, const Vector2D& desiredSize)
	{
		const sf::Vector2u textureSize = sprite.getTexture()->getSize();
		const sf::Vector2f spriteScale = { desiredSize.x / textureSize.x, desiredSize.y / textureSize.y };
		return spriteScale;
	}

	sf::Vector2f GetSpriteOrigin(const sf::Sprite& sprite, const Vector2D& relativePosition)
	{
		const sf::Vector2u textureSize = sprite.getTexture()->getSize();
		return { relativePosition.x * textureSize.x, relativePosition.y * textureSize.y };
	}

	sf::Vector2f GetOrigin(const sf::Shape& shape, const Vector2D& relativePosition)
	{
		const sf::Vector2u textureSize = shape.getTexture()->getSize();
		return { relativePosition.x * textureSize.x, relativePosition.y * textureSize.y };
	}

	sf::Vector2f GetTextOrigin(const sf::Text& text, const Vector2D& relativePosition)
	{
		sf::FloatRect textSize = text.getLocalBounds();
		return {
			(textSize.left + textSize.width) * relativePosition.x,
			(textSize.top + textSize.height) * relativePosition.y,
		};
	}

	bool HasMaskFlag(int mask, int flag)
	{
		return (mask & flag) == flag;
	}
}
