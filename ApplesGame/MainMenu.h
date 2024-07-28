#pragma once
#include "SFML/Graphics.hpp"
#include "Menu.h"

namespace ApplesGame
{
	struct MainMenu
	{
		TextField hintText;
		Checkbox infiniteAppleMenuItem;
		Checkbox withAccelerationMenuItem;
		TextField startGameText;
	};

	void DrawMainMenu(MainMenu& mainMenu, int gameMode, sf::RenderWindow& window);

	void InitMainMenu(MainMenu& mainMenu, const sf::Font& font);
}