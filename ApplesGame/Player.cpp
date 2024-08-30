#pragma once

#include "Player.h"
#include <assert.h>
#include "GameSettings.h"
#include "Game.h"
#include "Actor.h"
#include "Application.h"

namespace ApplesGame
{
	void Platform::Update(float timeDelta)
	{
		GameObject::Update(timeDelta);

		if (HasCollision()) {
			GameObject::UndoUpdate();
		}
	}

	void Platform::HandleInput()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			SetDirection({ 1.f, 0.f });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			SetDirection({ -1.f, 0.f });
		}
		else {
			SetDirection({ 0.f, 0.f });
		}
	}

	void Ball::Update(float timeDelta)
	{

		sf::Vector2f newDirection = direction;

		auto hasCollision = ((Circle*)shape)->HasCollision();
		if (hasCollision) {
			newDirection = reflectVector(position, ((Circle*)shape)->getRadius(), direction, { 0.f, 0.f }, { SCREEN_WIDTH, 0.f });

			if (newDirection == direction) {
				newDirection = reflectVector(position, ((Circle*)shape)->getRadius(), direction, { 0.f, 0.f }, { 0.f, SCREEN_HEGHT });
			}

			if (newDirection == direction) {
				newDirection = reflectVector(position, ((Circle*)shape)->getRadius(), direction, { SCREEN_WIDTH, 0.f }, { SCREEN_WIDTH, SCREEN_HEGHT });
			}

			if (newDirection == direction) {
				newDirection = reflectVector(position, ((Circle*)shape)->getRadius(), direction, { 0.f, SCREEN_HEGHT }, { SCREEN_WIDTH, SCREEN_HEGHT });
			}
		}


		SetDirection(newDirection);

		GameObject::Update(timeDelta);
	}
}
