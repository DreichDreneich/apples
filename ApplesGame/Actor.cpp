#pragma once
#include "Actor.h"
#include "Math.h"
#include "GameSettings.h"
#include "Application.h"
#include <math.h>

namespace ApplesGame
{
	void Actor::Draw()
	{
		sprite.setPosition(position);
		Application::Instance()->GetWindow().draw(sprite);
	}

	void Actor::Init(const sf::Texture& texture)
	{
		sprite.setTexture(texture);
		sprite.setOrigin(GetSpriteOrigin(sprite, { ORIGIN_MULTIPLIER, ORIGIN_MULTIPLIER })); // We need to use texture as origin ignores scale
		sprite.setScale(GetSpriteScale(sprite, { ACTOR_SIZE, ACTOR_SIZE }));
	}
}