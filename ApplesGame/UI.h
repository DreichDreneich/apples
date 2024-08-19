#pragma once
#include "SFML/Graphics.hpp"
#include "MainMenu.h"
#include "RecordsListPage.h"
#include "DifficultyPage.h"
#include "SettingsPage.h"

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
		// TODO: remove
		sf::RenderWindow* window;

		bool isBonusDurationVisible = false;

		void DrawHint();
		void DrawPauseMenu();

	public:
		Text scoreText;
		Text inputHintText;

		sf::Text bonusDuration;
		sf::Vector2f bonusDurationPosition;

		sf::Text gameTitle;

		MenuPage menuPage;
		DifficultyPage* difficultyPage;
		unique_ptr<SettingsPage> settingsPage;
		unique_ptr<RecordsList> recordsList;
		unique_ptr<GameOverPage> gameOverPage;

		unique_ptr<Page> currentPage;

		PauseGameMenu pauseGameMenu;

		UIState();

		void InitUI(sf::RenderWindow* window);
		void Update();
		void Draw();
	};
}
