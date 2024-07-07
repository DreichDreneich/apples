#pragma once
#include "Actor.h"
#include "Math.h"
#include "GameSettings.h"
#include <math.h>

namespace ApplesGame
{
	void InitActor(Actor& actor, const sf::Texture& texture)
	{
		// init stone state
		actor.position.x = (float)(rand() % (SCREEN_WIDTH + 1));
		actor.position.y = (float)(rand() % (SCREEN_HEGHT + 1));

		// Init sprite
		actor.sprite.setTexture(texture);
		actor.sprite.setOrigin(GetSpriteOrigin(actor.sprite, { 0.5f, 0.5f })); // We need to use texture as origin ignores scale
		actor.sprite.setScale(GetSpriteScale(actor.sprite, { ACTOR_SIZE, ACTOR_SIZE }));
	}

	bool HasActorCollisionWithCircleShape(const Actor& actor, const Position& circlePosition, const float circleDiameter)
	{
		float circleRadius = circleDiameter / 2.f;
		float actorHalfSide = ACTOR_SIZE / 2;
		float distancex = abs(circlePosition.x - actor.position.x);
		float distancey = abs(circlePosition.y - actor.position.y);
		float halfDistance = actorHalfSide + circleRadius;

		if (distancex > halfDistance || distancey > halfDistance)
		{
			return false;
		}

		if (distancex <= actorHalfSide || distancey <= actorHalfSide)
		{
			return true;
		}

		double distSq = pow(distancex - actorHalfSide, 2) + pow((distancey - actorHalfSide), 2);

		return distSq <= pow(circleRadius, 2);
	}

	void DrawActor(Actor& actor, sf::RenderWindow& window)
	{
		actor.sprite.setPosition(OurVectorToSf(actor.position));
		window.draw(actor.sprite);
	}
}