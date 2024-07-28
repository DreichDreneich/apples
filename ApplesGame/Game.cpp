#include <SFML/Audio.hpp>

#include "Game.h"
#include "GameField.h"

#include <assert.h>

#include <string>
#include <iostream>

using namespace std;


namespace ApplesGame
{
	void ToggleGameMode(State& gameState, int menuItem)
	{
		if (HasMaskFlag(gameState.gameMode, menuItem))
		{
			gameState.gameMode ^= menuItem;
		}
		else
		{
			gameState.gameMode |= menuItem;
		}
	}

	void GenerateRecordsList(State& gameState)
	{
		map<string, int>::iterator it = gameState.recordsList.begin();

		while (it != gameState.recordsList.end()) {
			it->second = rand() % 10;
			gameState.uiState.recordsList[it->first] = {};
			++it;
		}

		gameState.recordsList.insert({ PLAYER_NAME, 0 });
		gameState.uiState.recordsList.insert({ PLAYER_NAME, {} });
	}

	void InitGame(State& gameState)
	{
		gameState.gameMode = 0;
		gameState.gameState.push(GameState::MainMenu);

		gameState.actorsInfo[ActorType::APPLE].num = NUM_APPLES;
		gameState.actorsInfo[ActorType::APPLE].store.resize(NUM_APPLES);

		gameState.actorsInfo[ActorType::STONE].num = NUM_STONES;
		gameState.actorsInfo[ActorType::STONE].store.resize(NUM_STONES);

		gameState.actorsInfo[ActorType::BONUS].num = NUM_BONUSES;
		gameState.actorsInfo[ActorType::BONUS].store.resize(NUM_BONUSES);

		// Init game resources (terminate if error)
		assert(gameState.playerTexture.loadFromFile(RESOURCES_PATH + "Snake_Body.png"));
		assert(gameState.playerHeadTexture.loadFromFile(RESOURCES_PATH + "Snake_head.png"));
		assert(gameState.actorsInfo[ActorType::APPLE].texture.loadFromFile(RESOURCES_PATH + "Apple.png"));
		assert(gameState.actorsInfo[ActorType::STONE].texture.loadFromFile(RESOURCES_PATH + "Stone.png"));
		assert(gameState.actorsInfo[ActorType::BONUS].texture.loadFromFile(RESOURCES_PATH + "xyz-logo.png"));
		assert(gameState.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		LoadAndPrepareSound(gameState.deathSound, "Death.wav");
		LoadAndPrepareSound(gameState.applePickSound, "AppleEat.wav");
		LoadAndPrepareSound(gameState.bonusPickSound, "ding.flac");

		gameState.xCellsNum = SCREEN_WIDTH / FIELD_CELL_SIZE;
		gameState.yCellsNum = (SCREEN_HEGHT - (unsigned int)TOP_PADDING) / FIELD_CELL_SIZE;

		GenerateRecordsList(gameState);
		InitUI(gameState.uiState, gameState.font);
		RestartGame(gameState);
	}

	void GenerateNewActorPosition(State& state, GameEl& elem, int oldX, int oldY)
	{
		bool generated = false;

		while (!generated)
		{
			auto x = rand() % state.xCellsNum;
			auto y = rand() % state.yCellsNum;

			if (state.gameField.grid[x][y].type == ActorType::NONE)
			{
				state.gameField.grid[x][y] = elem;
				GameEl emptyGameEl{};
				state.gameField.grid[oldX][oldY] = emptyGameEl;

				generated = true;
			}
		}
	}

	void CreateActors(State& state, ActorType type)
	{
		int counter = 0;

		while (counter < state.actorsInfo[type].num)
		{
			auto x = rand() % state.xCellsNum;
			auto y = rand() % state.yCellsNum;

			if (state.gameField.grid[x][y].type == ActorType::NONE)
			{
				state.actorsInfo[type].store[counter].Init(state.actorsInfo[type].texture);

				GameEl el{};
				el.idx = counter;
				el.type = type;
				state.gameField.grid[x][y] = el;
				++counter;
			}
		}
	}

	void RestartGame(State& state)
	{
		auto middleX = state.xCellsNum / 2;
		auto middleY = state.yCellsNum / 2;

		state.gameField.grid.clear();
		state.gameField.grid.resize(state.xCellsNum);

		for (int i = 0; i < state.xCellsNum; i++)
		{
			state.gameField.grid[i].clear();
			state.gameField.grid[i].resize(state.yCellsNum);
		}

		state.player.Init({ middleX, middleY}, state.playerHeadTexture, state.playerTexture);

		CreateActors(state, ActorType::APPLE);
		CreateActors(state, ActorType::STONE);
 		CreateActors(state, ActorType::BONUS);

 		state.score = 0;
		state.timeSinceGameOver = 0.f;
	}

	void HandleInput(State& gameState)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && gameState.player.lastDirection != PlayerDirection::Down)
		{
			gameState.player.direction = PlayerDirection::Up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && gameState.player.lastDirection != PlayerDirection::Left)
		{
			gameState.player.direction = PlayerDirection::Right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && gameState.player.lastDirection != PlayerDirection::Up)
		{
			gameState.player.direction = PlayerDirection::Down;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && gameState.player.lastDirection != PlayerDirection::Right)
		{
			gameState.player.direction = PlayerDirection::Left;
		}
	}

	bool CheckFieldCell(State& gameState)
	{
		auto currentHeadPosition = gameState.player.partsPositions[0].position;

		auto isXOutOfBound = currentHeadPosition.x < 0 || currentHeadPosition.x > gameState.xCellsNum;
		auto isYOutOfBound = currentHeadPosition.y < 0 || currentHeadPosition.y > gameState.xCellsNum;

		if (isXOutOfBound || isYOutOfBound)
		{
			return true;
		}

		auto fieldCell = &gameState.gameField.grid[currentHeadPosition.x][currentHeadPosition.y];

		switch (fieldCell->type)
		{
		case ActorType::NONE:
		{
			for (int i = 1; i < gameState.player.partsPositions.size(); ++i) {
				if (currentHeadPosition == gameState.player.partsPositions[i].position) {
					return true;
				}
			}

			break;
		}
		case ActorType::APPLE:
		{
			if (HasMaskFlag(gameState.gameMode, (int)GameMode::infiniteApple)) {
				GenerateNewActorPosition(gameState, *fieldCell, currentHeadPosition.x, currentHeadPosition.y);
			}
			else {
				fieldCell->type = ActorType::NONE;
			}

			gameState.score = gameState.score + (gameState.player.hasBonus ? 2 : 1);

			if (HasMaskFlag(gameState.gameMode, (int)GameMode::withAcceleration)) {
				gameState.player.speed += ACCELERATION;
			}

			PlaySound(gameState.applePickSound.sound);
			gameState.player.AddPart();
			break;
		}
		case ActorType::STONE:
		{
			return true;
		}
		case ActorType::BONUS:
		{
			fieldCell->type = ActorType::NONE;

			gameState.player.hasBonus = true;
			gameState.player.bonusTimeRemaining = 10.f;
			PlaySound(gameState.bonusPickSound.sound);
			break;
		}
		default:
			return false;
			break;
		}

		return false;
	}

	void UpdateActors(State& gameState, float timeDelta)
	{
		if (gameState.gameState.top() == GameState::GameOverMenu) {
			gameState.timeSinceGameOver += timeDelta;
			return;
		}

		gameState.player.Update(timeDelta);

		if (gameState.player.hasBonus) {
			gameState.player.bonusTimeRemaining -= timeDelta;

			if ((int)gameState.player.bonusTimeRemaining <= 0)
			{
				gameState.player.hasBonus = false;
				gameState.player.size = INITIAL_PLAYER_SIZE;
			}
		}

		auto isGameOver = CheckFieldCell(gameState);

		if (isGameOver)
		{
			gameState.gameState.push(GameState::GameOverMenu);
			gameState.timeSinceGameOver = 0.f;

			auto prevRecord = gameState.recordsList.find(PLAYER_NAME);
			if (prevRecord->second < gameState.score)
			{
				prevRecord->second = gameState.score;
			}

			PlaySound(gameState.deathSound.sound);
		}
	}

	void UpdateGame(State& state, float timeDelta)
	{
		if (state.gameState.top() == GameState::Game)
		{
			UpdateActors(state, timeDelta);
		}

		UpdateUI(state.uiState, state);
	}

	void DrawGame(State& state, sf::RenderWindow& window)
	{
		if (state.gameState.top() == GameState::Game)
		{
			state.player.Draw(window);
			state.gameField.Draw(state, window);
		}

		DrawUI(state, window);
	}
}