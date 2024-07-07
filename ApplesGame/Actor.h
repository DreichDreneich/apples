#pragma once
#include "Math.h"
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
	struct Actor
	{
		Position position;
		sf::Sprite sprite;
	};

	void InitActor(Actor& actor, const sf::Texture& texture);

	bool HasActorCollisionWithCircleShape(const Actor& actor, const Position& circlePosition, const float circleDiameter);

	void DrawActor(Actor& actor, sf::RenderWindow& window);
}
