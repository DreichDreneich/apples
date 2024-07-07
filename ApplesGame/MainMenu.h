#pragma once
#include "SFML/Graphics.hpp"
#include "Menu.h"

namespace ApplesGame
{
	struct MainMenu
	{
		sf::Text hintText;
		MenuItem infiniteAppleMenuItem;
		MenuItem withAccelerationMenuItem;
		sf::Text startGameText;
	};

	void DrawMainMenu(MainMenu& mainMenu, int gameMode, sf::RenderWindow& window);

	void InitMainMenu(MainMenu& mainMenu, const sf::Font& font);
}