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

	Difficulty* State::getDifficulty()
	{
		return difficulty;
	}

	void State::setDifficulty(Difficulty value)
	{
		difficulty = &value;
	}

	Player* State::getPlayer() 
	{
		return &player;
	}

	GameField* State::getGameField()
	{
		return &gameField;
	}

	stack<GameState>* State::getGameState()
	{
		return &gameState;
	}

	void State::clearGameState()
	{
		gameState = {};
	}

	void State::HandleKeyboardEvent(const sf::Event& evt)
	{
		if (gameState.top() == GameState::MainMenu) {
			uiState.menuPage.HandleKeyboardEvent(evt);
		}

		if (gameState.top() == GameState::DifficultyPage) {
			uiState.difficultyPage->HandleKeyboardEvent(evt);
		}
	}

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

	void State::HandleKeyReleasedEvent(sf::Event event)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Num1:
		case sf::Keyboard::Numpad1:
		{
			if (gameState.top() == GameState::MainMenu)
			{
				State::Instance()->ToggleGameMode((int)GameMode::infiniteApple);
			}
			else if (gameState.top() == GameState::PauseMenu)
			{
				gameState = {};
				gameState.push(GameState::MainMenu);
			}
			break;
		}
		case sf::Keyboard::Num2:
		case sf::Keyboard::Numpad2:
		{
			if (gameState.top() == GameState::MainMenu)
			{
				State::Instance()->ToggleGameMode((int)GameMode::withAcceleration);
			}
			else if (gameState.top() == GameState::PauseMenu)
			{
				Application::Instance()->GetWindow().close();
			}
			break;
		}
		case sf::Keyboard::Num3:
		{
			if (gameState.top() == GameState::PauseMenu)
			{
				gameState.pop();
			}
			break;
		}
		case sf::Keyboard::Space:
		{
			if (gameState.top() == GameState::PauseMenu)
			{
				gameState.pop();
			}
			else if (gameState.top() == GameState::GameOverMenu)
			{
				State::Instance()->Restart();
				gameState.pop();
			}
			else if (gameState.top() == GameState::MainMenu)
			{
				State::Instance()->Restart();
				gameState = {};
				gameState.push(GameState::Game);
			}
			else if (gameState.top() == GameState::Game)
			{
				gameState.push(GameState::PauseMenu);
			}
			break;
		}
		case sf::Keyboard::Tab:
		{
			if (gameState.top() == GameState::MainMenu) {
				gameState.push(GameState::Records);
			}
			else if (gameState.top() == GameState::Records) {
				gameState.pop();
			}
			break;
		}
		default:
			break;
		}
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

			if (state->getGameField()->grid[x][y].type == ActorType::NONE)
			{
				state->actorsInfo[type].store[counter].Init(state->actorsInfo[type].texture);

				GameEl el{};
				el.idx = counter;
				el.type = type;
				state->getGameField()->grid[x][y] = el;
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

		auto isXOutOfBound = currentHeadPosition.x < 0 || currentHeadPosition.x >= xCellsNum;
		auto isYOutOfBound = currentHeadPosition.y < 0 || currentHeadPosition.y >= yCellsNum;

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

		this->uiState.Update();
	}

	void State::Draw()
	{
		if (gameState.top() == GameState::Game)
		{
			gameField.Draw();
			player.Draw();
		}

		uiState.Draw();
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

	void State::Init(sf::RenderWindow& window)
	{
		GenerateRecordsList();
		uiState.InitUI(&window);
	}
}