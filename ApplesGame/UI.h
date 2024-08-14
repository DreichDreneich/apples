#pragma once
#include "SFML/Graphics.hpp"
#include "MainMenu.h"
#include "RecordsListPage.h"
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

		void DrawHint();
		void DrawPauseMenu();

	public:
		Text scoreText;
		Text inputHintText;
		Text gameOverText;
		Text recordsListHeader;

		sf::Text bonusDuration;
		sf::Vector2f bonusDurationPosition;

		sf::Text gameTitle;

		MenuPage menuPage;
		DifficultyPage* difficultyPage;
		unique_ptr<RecordsList> recordsList1;
		unique_ptr<GameOverPage> gameOverPage;

		unique_ptr<Page> currentPage;

		PauseGameMenu pauseGameMenu;

		bool isBonusDurationVisible = false;

		UIState();

		void InitUI(sf::RenderWindow* window);
		void Update();
		void Draw();
	};
}
