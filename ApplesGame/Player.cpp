#pragma once

#include "Player.h"
#include <assert.h>
#include "GameSettings.h"
#include "Game.h"
#include "Actor.h"

namespace ApplesGame
{
	sf::Sprite InitSprite(const sf::Texture& texture, const float size)
	{
		sf::Sprite sprite{};

		sprite.setTexture(texture);
		sprite.setOrigin(GetSpriteOrigin(sprite, { ORIGIN_MULTIPLIER, ORIGIN_MULTIPLIER })); // We need to use texture as origin ignores scale
		sprite.setScale(GetSpriteScale(sprite, { size, size }));

		return sprite;
	}

	void Player::Init(const Vector2i& position, const sf::Texture& headTexture, const sf::Texture& texture)
	{
		partsPositions.clear();
		sprite.clear();

		for (short i = 0; i < 3; ++i)
		{
			Vector2i vector;
			vector.x = position.x;
			vector.y = position.y + i;
			PartPosition position(vector, PlayerDirection::Up);
			partsPositions.push_back(position);
		}

		Player::texture = texture;

		speed = INITIAL_SPEED;
		direction = PlayerDirection::Up;
		size = INITIAL_PLAYER_SIZE;
		hasBonus = false;
		bonusTimeRemaining = 10.f;

		for (int i = 0; i < partsPositions.size(); ++i)
		{
			auto new_sprite = InitSprite(i == 0 ? headTexture : texture, size);
			sprite.push_back(new_sprite);
		}
	}

	void Player::HandleInput()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && lastDirection != PlayerDirection::Down)
		{
			direction = PlayerDirection::Up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && lastDirection != PlayerDirection::Left)
		{
			direction = PlayerDirection::Right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && lastDirection != PlayerDirection::Up)
		{
			direction = PlayerDirection::Down;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && lastDirection != PlayerDirection::Right)
		{
			direction = PlayerDirection::Left;
		}
	}

	void Player::Update(float timeDelta)
	{
		nextSpeedUpdateTime += timeDelta;;

		if (speed * nextSpeedUpdateTime < 1.f) {
			return;
		}

		lastDirection = direction;

		prevTailPosition.direction = partsPositions.back().direction;
		prevTailPosition.position = partsPositions.back().position;
		partsPositions[0].direction = direction;

		auto prevPartPosition = partsPositions[0];

		for (int i = 1; i < partsPositions.size(); ++i) {
			auto temp = partsPositions[i];
			partsPositions[i].direction = prevPartPosition.direction;
			partsPositions[i].position = prevPartPosition.position;
			prevPartPosition = temp;
		}

		// Move player
		switch (direction)
		{
			case PlayerDirection::Up:
			{
				--partsPositions[0].position.y;
				break;
			}
			case PlayerDirection::Right:
			{
				++partsPositions[0].position.x;
				break;
			}
			case PlayerDirection::Down:
			{
				++partsPositions[0].position.y;
				break;
			}
			case PlayerDirection::Left:
			{
				--partsPositions[0].position.x;
				break;
			}
		}

		nextSpeedUpdateTime = 0.f;
	}

	void Player::AddPart()
	{
		partsPositions.push_back(prevTailPosition);
	}

	void Player::Draw(sf::RenderWindow& window)
	{
		auto newSpritesNum = partsPositions.size() - sprite.size();

		if (newSpritesNum > 0)
		{
			for (int i = 0; i < newSpritesNum; ++i)
			{
				sprite.push_back(InitSprite(texture, size));
			}
		}

		for (int i = 0; i < partsPositions.size(); ++i)
		{
			const sf::Vector2f spriteScale = (GetSpriteScale(sprite[i], { size, size }));

			auto direction = i == 0 ? Player::direction : partsPositions[i].direction;

			switch (direction)
			{
			case PlayerDirection::Up:
			{
				sprite[i].setScale(spriteScale.x, spriteScale.y);
				sprite[i].setRotation(-90.f);
				break;
			}
			case PlayerDirection::Right:
			{
				sprite[i].setScale(spriteScale.x, spriteScale.y);
				sprite[i].setRotation(0.f);
				break;
			}
			case PlayerDirection::Down:
			{
				sprite[i].setScale(spriteScale.x, spriteScale.y);
				sprite[i].setRotation(90.f);
				break;
			}
			case PlayerDirection::Left:
			{
				sprite[i].setScale(-spriteScale.x, spriteScale.y);
				sprite[i].setRotation(0.f);
				break;
			}
			}

			auto screenPosition = GameField::FieldToScreenPosition(partsPositions[i].position);
			
			if (i == 0)
			{
				position = screenPosition;
				if (hasBonus)
				{
					sprite[i].setColor(sf::Color::Yellow);
				}
			}

			sprite[i].setPosition(screenPosition);
			window.draw(sprite[i]);
		}
	}

	PartPosition::PartPosition(Vector2i pos, PlayerDirection dir)
	{
		position = pos;
		direction = dir;
	}
}
