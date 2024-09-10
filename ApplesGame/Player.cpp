#pragma once

#include <assert.h>
#include "Player.h"
#include "GameSettings.h"
#include "Game.h"
#include "Actor.h"
#include "Application.h"
#include "Utils.h"

namespace ApplesGame
{
	void Platform::Update(float timeDelta)
	{
		GameObject::Update(timeDelta);

		if (HasCollisionWithWindow()) {
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
		auto newDirection = direction;
		auto radius = ((Circle*)shape)->getRadius();
		auto hasCollision = CollisionManager::HasCollisionCircleWindow(position, radius);

		if (hasCollision) {
			newDirection = reflectVector(position, radius, direction, { 0.f, 0.f + TOP_PADDING }, { SCREEN_WIDTH, 0.f + TOP_PADDING });

			if (newDirection == direction) {
				newDirection = reflectVector(position, radius, direction, { 0.f, 0.f + TOP_PADDING }, { 0.f, SCREEN_HEGHT });
			}

			if (newDirection == direction) {
				newDirection = reflectVector(position, radius, direction, { SCREEN_WIDTH, 0.f + TOP_PADDING }, { SCREEN_WIDTH, SCREEN_HEGHT });
			}

			if (newDirection == direction) {
				State::Instance()->setGameOverState();
				newDirection = reflectVector(position, radius, direction, { 0.f, SCREEN_HEGHT }, { SCREEN_WIDTH, SCREEN_HEGHT });
			}
		}

		SetDirection(newDirection);

		GameObject::Update(timeDelta);
	}
}
