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
#include "DifficultyPage.h"

using namespace std;

namespace ApplesGame
{
	enum class GameMode : short
	{
		infiniteApple = 1, withAcceleration
	};

	enum class GameState : short {
		Game, MainMenu, PauseMenu, GameOverMenu, Records, DifficultyPage, SettingsPage
	};

	class Settings {
		bool isMusicOn;
		bool isSoundOn;

	public:
		bool* getIsMusicOn() { return &isMusicOn; }
		bool* getisSoundOn() { return &isSoundOn; }
	};

	class State
	{
	private:
		State();
		static State* _instance;
		void GenerateRecordsList();
		void UpdateActors(float timeDelta);
		bool CheckFieldCell();
		void GenerateNewActorPosition(GameEl& elem, int oldX, int oldY);

		map<Difficulty, float> accelerationByDifficulty = {
			{Difficulty::EASY, 0.f},
			{Difficulty::MEDIUM, 0.5f},
			{Difficulty::HARD, 1.f},
		};

		Difficulty difficulty;
		Player player;
		GameField gameField;
		UIState uiState;
		stack<GameState> gameState;


	public:
		Difficulty* getDifficulty();
		void setDifficulty(Difficulty);

		Player* getPlayer();

		GameField* getGameField();

		stack<GameState>* getGameState();
		void clearGameState();

		State(State& other) = delete;
		State operator=(const State&) = delete;

		~State();

		static State* Instance();

		void Init(sf::RenderWindow& window);

		map<ActorType, ActorInfo> actorsInfo;

		Settings setings;
		SoundManager* soundManager;

		// Game resources
		sf::Font font;
		sf::Texture playerTexture;
		sf::Texture playerHeadTexture;
		sf::Texture appleTexture;
		sf::Texture stoneTexture;
		sf::Texture bonusTexture;

		int score = 0;
		float timeSinceGameOver = 0.f;

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

		void HandleKeyReleasedEvent(sf::Event event);
		void HandleKeyboardEvent(const sf::Event& evt);
		void ToggleGameMode(int menuItem);
		void Restart();
		void Update(float timeDelta);
		void Draw();
	};
}
