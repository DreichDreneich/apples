#pragma once

#include <SFML/Graphics.hpp>

#include "Utils.h"
#include "GameSettings.h"

namespace ApplesGame {
	struct Line
	{
		sf::Vector2f p1;
		sf::Vector2f p2;
	};

	class CollisionManager {
	public:
		static bool HasCollisionCircleRect(const sf::CircleShape& circle, const sf::RectangleShape& rect);

		static bool HasCollisionCircleWindow(sf::Vector2f position, float radius);
	};

	sf::Vector2f reflectVector(sf::Vector2f circleCenter, float r, sf::Vector2f direction, sf::Vector2f point1, sf::Vector2f point2);

	std::vector<Line>::const_iterator findIntersectionCircleRectangle(sf::Vector2f circleCenter, float r, const std::vector<Line>& lines);

	const std::vector<Line> GetRectLines(const sf::RectangleShape& rect, sf::Vector2f position);

	std::string generate_uuid();
}