#pragma once
#include "SFML/Graphics.hpp"
#include "Checkbox.h"
#include "Menu.h"
#include "Page.h"

namespace ApplesGame
{
	//struct MainMenu
	//{
	//	TextField hintText;
	//	Checkbox infiniteAppleMenuItem;
	//	Checkbox withAccelerationMenuItem;
	//	TextField startGameText;
	//};

	//void DrawMainMenu(MainMenu& mainMenu, int gameMode, sf::RenderWindow& window);

	//void InitMainMenu(MainMenu& mainMenu);


	class MenuPage : Page {
		Menu* menu;
		RadioMenu* rrmenu;
		string selectedDifficulty;
	public:
		MenuPage() = default;

		void Init();
		void HandleKeyboardEvent(const sf::Event& event);
		void Draw();
	};
}