#pragma once
#include "SFML/Graphics.hpp"
#include "MainMenu.h"

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

	struct UIState
	{
		Text scoreText;
		Text inputHintText;
		Text gameOverText;
		Text recordsListHeader;
		map<string, pair<Text, Text>> recordsList;


		sf::Text bonusDuration;
		sf::Vector2f bonusDurationPosition;

		sf::Text gameTitle;
		MainMenu mainMenu;

		PauseGameMenu pauseGameMenu;

		bool isBonusDurationVisible = false;
	};

	void InitUI(UIState& uiState, const sf::Font& font);
	void UpdateUI(UIState& uiState, const struct State& gameState);
	void DrawUI(const State& gamestate, sf::RenderWindow& window);
	void DrawHint(ApplesGame::UIState& uiState, sf::RenderWindow& window);
	void InitMenuItem(MenuItem& menuItem, sf::String str, const sf::Font& font);
}
