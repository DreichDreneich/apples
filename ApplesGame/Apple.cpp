#include "Apple.h"
#include "GameSettings.h"
#include "Player.h"
#include <cstdlib>
#include "2dTree.h"

namespace ApplesGame
{
	void InitApple(Apple& apple, const sf::Texture& texture)
	{
		apple.speed = 50.f; // TODO: move to parameters

		// init apple state
		apple.position.x = (float)(rand() % (SCREEN_WIDTH + 1));
		apple.position.y = (float)(rand() % (SCREEN_HEGHT + 1));

		// Init sprite
		apple.sprite.setTexture(texture);
		apple.sprite.setOrigin(GetSpriteOrigin(apple.sprite, { 0.5f, 0.5f })); // We need to use texture as origin ignores scale
		apple.sprite.setScale(GetSpriteScale(apple.sprite, { APPLE_SIZE, APPLE_SIZE }));
	}

	void UpdateApple(Apple& apple, Player& player, float timeDelta)
	{
		bool hasScreenCollision = HasCircleShapeCollisionWithScreenBorder(apple.position, APPLE_SIZE);

		switch (player.direction)
		{
		case PlayerDirection::Up:
		case PlayerDirection::Down:
		{
			if (!hasScreenCollision && player.position.x < apple.position.x)
			{
				apple.position.x += apple.speed * timeDelta;
			}
			else if (!hasScreenCollision && player.position.x > apple.position.x)
			{
				apple.position.x -= apple.speed * timeDelta;
			}
			else 
			{
				apple.position.y -= apple.speed * timeDelta;
			}
			break;
		}
		case PlayerDirection::Right:
		case PlayerDirection::Left:
		{
			if (!hasScreenCollision && player.position.y < apple.position.y)
			{
				apple.position.y += apple.speed * timeDelta;
			}
			else if (!hasScreenCollision && player.position.y > apple.position.y)
			{
				apple.position.y -= apple.speed * timeDelta;
			}
			else 
			{
				apple.position.x += apple.speed * timeDelta;
			}
			break;
		}
		}
	}

	void DrawApple(Apple& apple, sf::RenderWindow& window)
	{
		apple.sprite.setPosition(OurVectorToSf(apple.position));
		window.draw(apple.sprite);
	}
}
