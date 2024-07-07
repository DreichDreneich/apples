#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Player.h"
#include "Apple.h"
#include "UI.h"
#include "GameSettings.h"
#include "Actor.h"
#include "Sound.h"
#include "2dTree.h"
#include <stack>

using namespace std;

namespace ApplesGame
{
	enum class GameMode : short
	{
		infiniteApple = 1, withAcceleration
	};

	enum class GameState : short {
		Game, MainMenu, PauseMenu, GameOverMenu, Records
	};

	struct State
	{
		Player player;
		Apple apples[NUM_APPLES];
		NodePtr applesTree;
		Actor stones[NUM_STONES];
		Actor bonuses[NUM_BONUSES];

		UIState uiState;

		// Game resources
		sf::Font font;
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture stoneTexture;
		sf::Texture bonusTexture;

		SoundEntity deathSound;
		SoundEntity bonusPickSound;
		SoundEntity applePickSound;

		int numEatenApples = 0;
		//bool isGameOver = false;
		float timeSinceGameOver = 0.f;

		stack<GameState> gameState;

		map<string, int> recordsList = {
			{"Petr", 0},
			{"Svetlana", 0},
			{"Nikolay", 0},
			{"Olga", 0},
		};

		// 1 - with infinite apples
		// 2 - with acceleration after apple eating
		int gameMode : 3;
	};

	void ToggleGameMode(State& gameState, int menuItem);
	void InitGame(State& gameState);
	void RestartGame(State& gameState);
	void HandleInput(State& gameState);
	void UpdateGame(State& gameState, float timeDelta);
	void DrawGame(State& gameState, sf::RenderWindow& window);
	void DrawGameEnvironment(State& gameState, sf::RenderWindow& window);
}
