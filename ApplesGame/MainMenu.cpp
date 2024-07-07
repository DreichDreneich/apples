#pragma once
#include "MainMenu.h"
#include "Math.h"
#include "Menu.h"
#include "Game.h"
#include "UI.h"

namespace ApplesGame
{
	void InitMainMenu(MainMenu& mainMenu, const sf::Font& font)
	{
		mainMenu.hintText.setFont(font);
		mainMenu.hintText.setCharacterSize(32);
		mainMenu.hintText.setFillColor(sf::Color::Yellow);
		mainMenu.hintText.setString("Choose game modes by pressing <numbers>:");
		mainMenu.hintText.setOrigin(GetTextOrigin(mainMenu.hintText, { 0.5f, 0.5f }));

		InitMenuItem(mainMenu.infiniteAppleMenuItem, "<1>  With infinite apples", font);
		InitMenuItem(mainMenu.withAccelerationMenuItem, "<2>  With acceleration after apple eating", font);

		mainMenu.startGameText.setFont(font);
		mainMenu.startGameText.setCharacterSize(32);
		mainMenu.startGameText.setFillColor(sf::Color::Yellow);
		mainMenu.startGameText.setString("Press <Space> to start the game");
		mainMenu.startGameText.setOrigin(GetTextOrigin(mainMenu.startGameText, { 0.5f, 0.5f }));
	}

	void CalculateMenuItem(MenuItem& menuItem, GameMode menuType, int gameMode, Position position)
	{
		float checkboxMargin = 20;

		menuItem.textObj.setPosition(OurVectorToSf({ position.x + checkboxMargin, position.y }));
		sf::FloatRect textRect = menuItem.textObj.getGlobalBounds();
		menuItem.checkbox.setPosition({ textRect.left - checkboxMargin, textRect.top + CheckboxSize / 2 });

		sf::Color color = HasMaskFlag(gameMode, (int)menuType)
			? sf::Color::Yellow
			: sf::Color::Black;
		menuItem.checkbox.setFillColor(color);
	}

	void DrawMainMenu(MainMenu& mainMenu, int gameMode, sf::RenderWindow& window)
	{
		float windowX = (float)window.getSize().x;
		float windowY = (float)window.getSize().y;

		mainMenu.hintText.setPosition(windowX / 2, 170.f);
		mainMenu.startGameText.setPosition(windowX / 2, windowY - 100.f);

		sf::FloatRect hintTextRect = mainMenu.hintText.getGlobalBounds();

		CalculateMenuItem(mainMenu.infiniteAppleMenuItem, GameMode::infiniteApple, gameMode, { hintTextRect.left, 230.f });
		CalculateMenuItem(mainMenu.withAccelerationMenuItem, GameMode::withAcceleration, gameMode, { hintTextRect.left, 265.f });

		window.draw(mainMenu.hintText);
		window.draw(mainMenu.infiniteAppleMenuItem.checkbox);
		window.draw(mainMenu.infiniteAppleMenuItem.textObj);
		window.draw(mainMenu.withAccelerationMenuItem.checkbox);
		window.draw(mainMenu.withAccelerationMenuItem.textObj);
		window.draw(mainMenu.startGameText);
	}
}