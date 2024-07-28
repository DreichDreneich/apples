#pragma once
#include "Math.h"
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
	struct Actor
	{
		sf::Vector2f position;
		sf::Sprite sprite;

		void Draw(sf::RenderWindow& window) {
			sprite.setPosition(position);
			window.draw(sprite);
		}

		void Init(const sf::Texture& texture);
	};
}
