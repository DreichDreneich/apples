#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stack>

#include "Player.h"
#include "UI.h"
#include "GameSettings.h"
#include "Sound.h"
#include "GameField.h"
#include "ActorInfo.h"

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

	class State
	{
	private:
		State();
		//~State() {};
		static State* _instance;
		void GenerateRecordsList();
		void UpdateActors(float timeDelta);
		bool CheckFieldCell();
		void GenerateNewActorPosition(GameEl& elem, int oldX, int oldY);

	public:
		State(State& other) = delete;
		State operator=(const State&) = delete;

		~State();

		static State* Instance();

		Player player;
		GameField gameField;

		UIState uiState;

		map<ActorType, ActorInfo> actorsInfo;

		// Game resources
		sf::Font font;
		sf::Texture playerTexture;
		sf::Texture playerHeadTexture;
		sf::Texture appleTexture;
		sf::Texture stoneTexture;
		sf::Texture bonusTexture;

		SoundEntity deathSound;
		SoundEntity bonusPickSound;
		SoundEntity applePickSound;

		int score = 0;
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

		int xCellsNum;
		int yCellsNum;

		void ToggleGameMode(int menuItem);
		void Restart();
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);
	};
}
