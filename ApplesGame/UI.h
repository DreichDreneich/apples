#pragma once
#include "SFML/Graphics.hpp"
#include "MainMenu.h"
#include "Menu.h"

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
	public:
		Text scoreText;
		Text inputHintText;
		Text gameOverText;
		Text recordsListHeader;
		map<string, pair<Text, Text>> recordsList;


		sf::Text bonusDuration;
		sf::Vector2f bonusDurationPosition;

		sf::Text gameTitle;
		MainMenu mainMenu;
		MenuPage menuPage;

		PauseGameMenu pauseGameMenu;

		bool isBonusDurationVisible = false;

		void InitUI(const sf::Font& font);
		void Update(const struct State& gameState);
		void Draw(const State& gamestate, sf::RenderWindow& window);
	};
}
