#pragma once
#include "Actor.h"
#include "Math.h"
#include "GameSettings.h"
#include "Application.h"

namespace ApplesGame
{
	void GameObject::SetDirection(const sf::Vector2f& direction)
	{
		GameObject::direction = direction;
	}

	void GameObject::Update(float timeDelta)
	{
		prevPosition = position;
		position.x += speed * timeDelta * direction.x;
		position.y += speed * timeDelta * direction.y;
		shape->setPosition(position);
		//shape->setOrigin(GetOrigin(*shape, { ORIGIN_MULTIPLIER, ORIGIN_MULTIPLIER }));
	}

	void GameObject::UndoUpdate()
	{
		position = prevPosition;
		shape->setPosition(position);
		//shape->setOrigin(GetOrigin(*shape, { ORIGIN_MULTIPLIER, ORIGIN_MULTIPLIER }));
	}

	void GameObject::SetSpeed(const float& speed)
	{
		GameObject::speed = speed;
	}

	void GameObject::Draw()
	{
		Application::Instance()->GetWindow().draw(*shape);
	}
}