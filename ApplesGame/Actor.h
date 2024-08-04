#pragma once
#include "Math.h"
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
	struct Actor
	{
		sf::Vector2f position;
		sf::Sprite sprite;

		void Draw();

		void Init(const sf::Texture& texture);
	};
}
