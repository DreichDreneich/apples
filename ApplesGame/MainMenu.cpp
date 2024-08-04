#pragma once
#include "MainMenu.h"
#include "Math.h"
#include "Checkbox.h"
#include "Game.h"
#include "UI.h"

namespace ApplesGame
{
	void InitMainMenu(MainMenu& mainMenu)
	{
		mainMenu.hintText.Init("Choose game modes by pressing <numbers>:", State::Instance()->font);

		mainMenu.infiniteAppleMenuItem.Init("<1>  With infinite apples", State::Instance()->font);
		mainMenu.withAccelerationMenuItem.Init("<2>  With acceleration after apple eating", State::Instance()->font);

		mainMenu.startGameText.Init("Press <Space> to start the game", State::Instance()->font);
	}

	void CalculateMenuItem(Checkbox& menuItem, GameMode menuType, int gameMode, Position position)
	{
		float checkboxMargin = 20;

		menuItem.textObj.setPosition(OurVectorToSf({ position.x + checkboxMargin, position.y }));
		sf::FloatRect textRect = menuItem.textObj.getGlobalBounds();
		menuItem.checkboxRect.setPosition({ textRect.left - checkboxMargin, textRect.top + CheckboxSize / 2 });

		sf::Color color = HasMaskFlag(gameMode, (int)menuType)
			? sf::Color::Yellow
			: sf::Color::Black;
		menuItem.checkboxRect.setFillColor(color);
	}

	void DrawMainMenu(MainMenu& mainMenu, int gameMode, sf::RenderWindow& window)
	{
		float windowX = (float)window.getSize().x;
		float windowY = (float)window.getSize().y;

		mainMenu.hintText.textObj.setPosition(windowX / 2, 170.f);
		mainMenu.startGameText.textObj.setPosition(windowX / 2, windowY - 100.f);

		sf::FloatRect hintTextRect = mainMenu.hintText.textObj.getGlobalBounds();

		CalculateMenuItem(mainMenu.infiniteAppleMenuItem, GameMode::infiniteApple, gameMode, { hintTextRect.left, 230.f });
		CalculateMenuItem(mainMenu.withAccelerationMenuItem, GameMode::withAcceleration, gameMode, { hintTextRect.left, 265.f });

		window.draw(mainMenu.hintText.textObj);
		mainMenu.infiniteAppleMenuItem.Draw(window);
		mainMenu.withAccelerationMenuItem.Draw(window);
		window.draw(mainMenu.startGameText.textObj);
	}
}