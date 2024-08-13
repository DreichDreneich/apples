#pragma once
#include "SFML/Graphics.hpp"
#include "MainMenu.h"
#include "DifficultyPage.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	struct PauseGameMenu
	{
		Text mainMenu;
		Text quitGame;
		Text resumeGame;
	};

	class UIState
	{
		sf::RenderWindow* window;

		void DrawRecordsList(float topMargin = 0);
		void DrawGameOverScreen();
		void DrawHint();
		void DrawPauseMenu();

	public:
		Text scoreText;
		Text inputHintText;
		Text gameOverText;
		Text recordsListHeader;
		map<string, pair<Text, Text>> recordsList;

		sf::Text bonusDuration;
		sf::Vector2f bonusDurationPosition;

		sf::Text gameTitle;

		MenuPage menuPage;
		DifficultyPage* difficultyPage;

		PauseGameMenu pauseGameMenu;

		bool isBonusDurationVisible = false;

		UIState();

		void InitUI(sf::RenderWindow* window);
		void Update();
		void Draw();
	};
}
