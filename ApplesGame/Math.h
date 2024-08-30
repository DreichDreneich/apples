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

	struct Line
	{
		sf::Vector2f p1;
		sf::Vector2f p2;
	};

	typedef Vector2D Position;

	sf::Vector2f OurVectorToSf(const Vector2D& v);
	sf::Vector2f GetScale(const sf::Shape& shape, const Vector2D& desiredSize);
	sf::Vector2f GetSpriteScale(const sf::Sprite& sprite, const Vector2D& desiredSize);
	sf::Vector2f GetSpriteOrigin(const sf::Sprite& sprite, const Vector2D& relativePosition);
	sf::Vector2f GetOrigin(const sf::Shape& shape, const Vector2D& relativePosition);
	sf::Vector2f GetTextOrigin(const sf::Text& text, const Vector2D& relativePosition);
	bool HasMaskFlag(int mask, int value);
	sf::Vector2f reflectVector(sf::Vector2f circleCenter, float r, sf::Vector2f direction, sf::Vector2f point1, sf::Vector2f point2);
	std::vector<Line>::const_iterator findIntersectionCircleRectangle(sf::Vector2f circleCenter, float r, const std::vector<Line>& lines);
	std::vector<Line> GetRectLines(const sf::RectangleShape& rect, sf::Vector2f position);
}
