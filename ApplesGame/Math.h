#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace ApplesGame
{
	struct Vector2D
	{
		float x = 0.f;
		float y = 0.f;
	};


	typedef Vector2D Position;

	sf::Vector2f OurVectorToSf(const Vector2D& v);
	sf::Vector2f GetScale(const sf::Shape& shape, const Vector2D& desiredSize);
	sf::Vector2f GetSpriteScale(const sf::Sprite& sprite, const Vector2D& desiredSize);
	sf::Vector2f GetSpriteOrigin(const sf::Sprite& sprite, const Vector2D& relativePosition);
	sf::Vector2f GetOrigin(const sf::Shape& shape, const Vector2D& relativePosition);
	sf::Vector2f GetTextOrigin(const sf::Text& text, const Vector2D& relativePosition);
	bool HasMaskFlag(int mask, int value);
}
