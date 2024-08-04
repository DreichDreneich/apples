#pragma once
#include <SFML/Audio.hpp>

#include "Game.h"
#include "GameField.h"

#include <assert.h>

#include <string>
#include <iostream>

using namespace std;

namespace ApplesGame
{
	State* State::_instance;

	void State::ToggleGameMode(int menuItem)
	{
		if (HasMaskFlag(gameMode, menuItem))
		{
			gameMode ^= menuItem;
		}
		else
		{
			gameMode |= menuItem;
		}
	}

	void State::GenerateRecordsList()
	{
		map<string, int>::iterator it = recordsList.begin();

		while (it != recordsList.end()) {
			it->second = rand() % 10;
			uiState.recordsList[it->first] = {};
			++it;
		}

		recordsList.insert({ PLAYER_NAME, 0 });
		uiState.recordsList.insert({ PLAYER_NAME, {} });
	}

	void State::GenerateNewActorPosition(GameEl& elem, int oldX, int oldY)
	{
		bool generated = false;

		while (!generated)
		{
			auto x = rand() % xCellsNum;
			auto y = rand() % yCellsNum;

			if (gameField.grid[x][y].type == ActorType::NONE)
			{
				gameField.grid[x][y] = elem;
				GameEl emptyGameEl{};
				gameField.grid[oldX][oldY] = emptyGameEl;

				generated = true;
			}
		}
	}

	void CreateActors(ActorType type)
	{
		auto state = State::Instance();
		int counter = 0;

		while (counter < state->actorsInfo[type].num)
		{
			auto x = rand() % state->xCellsNum;
			auto y = rand() % state->yCellsNum;

			if (state->gameField.grid[x][y].type == ActorType::NONE)
			{
				state->actorsInfo[type].store[counter].Init(state->actorsInfo[type].texture);

				GameEl el{};
				el.idx = counter;
				el.type = type;
				state->gameField.grid[x][y] = el;
				++counter;
			}
		}
	}

	void State::Restart()
	{
		auto middleX = xCellsNum / 2;
		auto middleY = yCellsNum / 2;

		gameField.grid.clear();
		gameField.grid.resize(xCellsNum);

		for (int i = 0; i < xCellsNum; i++)
		{
			gameField.grid[i].clear();
			gameField.grid[i].resize(yCellsNum);
		}

		player.Init({ middleX, middleY}, playerHeadTexture, playerTexture);

		CreateActors(ActorType::APPLE);
		CreateActors(ActorType::STONE);
 		CreateActors(ActorType::BONUS);

 		score = 0;
		timeSinceGameOver = 0.f;
	}

	bool State::CheckFieldCell()
	{
		auto currentHeadPosition = player.partsPositions[0].position;

		auto isXOutOfBound = currentHeadPosition.x < 0 || currentHeadPosition.x > xCellsNum;
		auto isYOutOfBound = currentHeadPosition.y < 0 || currentHeadPosition.y > xCellsNum;

		if (isXOutOfBound || isYOutOfBound)
		{
			return true;
		}

		auto fieldCell = &gameField.grid[currentHeadPosition.x][currentHeadPosition.y];

		switch (fieldCell->type)
		{
		case ActorType::NONE:
		{
			for (int i = 1; i < player.partsPositions.size(); ++i) {
				if (currentHeadPosition == player.partsPositions[i].position) {
					return true;
				}
			}

			break;
		}
		case ActorType::APPLE:
		{
			if (HasMaskFlag(gameMode, (int)GameMode::infiniteApple)) {
				GenerateNewActorPosition(*fieldCell, currentHeadPosition.x, currentHeadPosition.y);
			}
			else {
				fieldCell->type = ActorType::NONE;
			}

			score = score + (player.hasBonus ? 2 : 1);

			if (HasMaskFlag(gameMode, (int)GameMode::withAcceleration)) {
				player.speed += ACCELERATION;
			}

			PlaySound(applePickSound.sound);
			player.AddPart();
			break;
		}
		case ActorType::STONE:
		{
			return true;
		}
		case ActorType::BONUS:
		{
			fieldCell->type = ActorType::NONE;

			player.hasBonus = true;
			player.bonusTimeRemaining = 10.f;
			PlaySound(bonusPickSound.sound);
			break;
		}
		default:
			return false;
			break;
		}

		return false;
	}

	void State::UpdateActors(float timeDelta)
	{
		if (gameState.top() == GameState::GameOverMenu) {
			timeSinceGameOver += timeDelta;
			return;
		}

		player.Update(timeDelta);

		if (player.hasBonus) {
			player.bonusTimeRemaining -= timeDelta;

			if ((int)player.bonusTimeRemaining <= 0)
			{
				player.hasBonus = false;
				player.size = INITIAL_PLAYER_SIZE;
			}
		}

		auto isGameOver = CheckFieldCell();

		if (isGameOver)
		{
			gameState.push(GameState::GameOverMenu);
			timeSinceGameOver = 0.f;

			auto prevRecord = recordsList.find(PLAYER_NAME);
			if (prevRecord->second < score)
			{
				prevRecord->second = score;
			}

			PlaySound(deathSound.sound);
		}
	}

	void State::Update(float timeDelta)
	{
		if (gameState.top() == GameState::Game)
		{
			UpdateActors(timeDelta);
		}

		UpdateUI(this->uiState, *this);
	}

	void State::Draw(sf::RenderWindow& window)
	{
		if (gameState.top() == GameState::Game)
		{
			player.Draw(window);
			gameField.Draw(*this, window);
		}

		DrawUI(*this, window);
	}

	State* State::Instance()
	{
		if (_instance == nullptr) {
			_instance = new State();
			return _instance;
		}

		return _instance;
	}

	State::~State()
	{
		delete State::_instance;
	}

	State::State()
	{
		gameMode = 0;
		gameState.push(GameState::MainMenu);

		actorsInfo[ActorType::APPLE].num = NUM_APPLES;
		actorsInfo[ActorType::APPLE].store.resize(NUM_APPLES);

		actorsInfo[ActorType::STONE].num = NUM_STONES;
		actorsInfo[ActorType::STONE].store.resize(NUM_STONES);

		actorsInfo[ActorType::BONUS].num = NUM_BONUSES;
		actorsInfo[ActorType::BONUS].store.resize(NUM_BONUSES);

		// Init game resources (terminate if error)
		assert(playerTexture.loadFromFile(RESOURCES_PATH + "Snake_Body.png"));
		assert(playerHeadTexture.loadFromFile(RESOURCES_PATH + "Snake_head.png"));
		assert(actorsInfo[ActorType::APPLE].texture.loadFromFile(RESOURCES_PATH + "Apple.png"));
		assert(actorsInfo[ActorType::STONE].texture.loadFromFile(RESOURCES_PATH + "Stone.png"));
		assert(actorsInfo[ActorType::BONUS].texture.loadFromFile(RESOURCES_PATH + "xyz-logo.png"));
		assert(font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		LoadAndPrepareSound(deathSound, "Death.wav");
		LoadAndPrepareSound(applePickSound, "AppleEat.wav");
		LoadAndPrepareSound(bonusPickSound, "ding.flac");

		xCellsNum = SCREEN_WIDTH / FIELD_CELL_SIZE;
		yCellsNum = (SCREEN_HEGHT - (unsigned int)TOP_PADDING) / FIELD_CELL_SIZE;
	}

	void State::Init()
	{
		GenerateRecordsList();
		InitUI(uiState, font);
	}
}