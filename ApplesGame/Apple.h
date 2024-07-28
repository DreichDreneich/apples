#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Player.h"
#include "Actor.h"

namespace ApplesGame
{
	struct Apple : Actor
	{
		Position position;
		sf::Sprite sprite;
	};

	void UpdateApple(Apple& apple, Player& player, float timeDelta);

	void InitApple(Apple& apple, const sf::Texture& texture);
	void DrawApple(Apple& apple, sf::RenderWindow& window);
}
