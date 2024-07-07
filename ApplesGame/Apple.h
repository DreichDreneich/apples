#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Player.h"

namespace ApplesGame
{
	struct Point {
		int x, y;
	};

	struct Apple
	{
		Position position;
		sf::Sprite sprite;
		float speed;
	};

	void UpdateApple(Apple& apple, Player& player, float timeDelta);

	void InitApple(Apple& apple, const sf::Texture& texture);
	void DrawApple(Apple& apple, sf::RenderWindow& window);
}
