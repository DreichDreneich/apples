#pragma once
#include "SFML/Graphics.hpp"

namespace ApplesGame
{
	const float CheckboxSize = 10.f;

	struct MenuItem {
		sf::Text textObj;
		bool isChecked;
		sf::RectangleShape checkbox;
	};
}