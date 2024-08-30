#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stack>

#include "Player.h"
#include "UI.h"
#include "GameSettings.h"
#include "Sound.h"
#include "ActorInfo.h"
#include "DifficultyPage.h"

using namespace std;

namespace ApplesGame
{
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

		map<Difficulty, float> accelerationByDifficulty = {
			{Difficulty::EASY, 0.f},
			{Difficulty::MEDIUM, 0.5f},
			{Difficulty::HARD, 1.f},
		};

		Difficulty difficulty;
		UIState uiState;
		stack<GameState> gameState;
		Font font;

		int score = 0;
		int xCellsNum;
		int yCellsNum;

	public:
		Difficulty* getDifficulty();
		void setDifficulty(Difficulty);

		Font& GetFont();

		stack<GameState>* getGameState();
		void clearGameState();

		State(State& other) = delete;
		State operator=(const State&) = delete;

		~State();

		static State* Instance();

		void Init(sf::RenderWindow& window);

		Settings setings;
		SoundManager* soundManager;

		float timeSinceGameOver = 0.f;

		map<string, int> recordsList = {
			{"Petr", 0},
			{"Svetlana", 0},
			{"Nikolay", 0},
			{"Olga", 0},
		};

		vector<GameObject*> gameObjects;
		Platform* platform;
		Ball* ball;

		int GetScore();
		void HandleKeyReleasedEvent(sf::Event event);
		void HandleKeyboardEvent(const sf::Event& evt);
		void Restart();
		void Update(float timeDelta);
		void Draw();
	};
}
