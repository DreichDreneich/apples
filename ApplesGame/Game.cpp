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
		return &difficulty;
	}

	void State::setDifficulty(Difficulty value)
	{
		difficulty = value;
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
		switch (gameState.top())
		{
		case GameState::MainMenu:
			uiState.menuPage.HandleKeyboardEvent(evt);
			break;
		case GameState::DifficultyPage:
			uiState.difficultyPage->HandleKeyboardEvent(evt);
			break;
		case GameState::GameOverMenu:
			uiState.gameOverPage->HandleKeyboardEvent(evt);
			break;
		case GameState::SettingsPage:
			uiState.settingsPage->HandleKeyboardEvent(evt);
			break;
		default:
			break;
		}
	}

	void State::GenerateRecordsList()
	{
		map<string, int>::iterator it = recordsList.begin();

		while (it != recordsList.end()) {
			it->second = rand() % 10;
			++it;
		}

		recordsList.insert({ PLAYER_NAME, 0 });
	}

	// TODO: move handlers into separate class
	void State::HandleKeyReleasedEvent(sf::Event event)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Num1:
		case sf::Keyboard::Numpad1:
		{
			if (gameState.top() == GameState::PauseMenu)
			{
				gameState = {};
				gameState.push(GameState::MainMenu);
			}
			break;
		}
		case sf::Keyboard::Num2:
		case sf::Keyboard::Numpad2:
		{
			if (gameState.top() == GameState::PauseMenu)
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
			else if (gameState.top() == GameState::Game)
			{
				gameState.push(GameState::PauseMenu);
			}
			break;
		}
		case sf::Keyboard::Tab:
		{
			if (gameState.top() == GameState::Records) {
				gameState.pop();
			}
			break;
		}
		default:
			break;
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
			/*if (difficulty != Difficulty::EASY)
				GenerateNewActorPosition(*fieldCell, currentHeadPosition.x, currentHeadPosition.y);
			else {*/
				fieldCell->type = ActorType::NONE;
			//}

			score = score + (player.hasBonus ? 2 : 1);

			player.speed += accelerationByDifficulty[difficulty];

			soundManager->Play(Sounds::ApplePickSound);
			
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
			soundManager->Play(Sounds::BonusPickSound);
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

			soundManager->Play(Sounds::DeathSound);
		}
	}

	Font& State::GetFont() {
		return font;
	}

	int State::GetScore()
	{
		return score;
	}

	void State::Update(float timeDelta)
	{
		if (gameState.top() == GameState::Game)
		{
			auto ballShape = ball->GetShape();
			auto platformLines = platform->GetLines();

			auto line = findIntersectionCircleRectangle(ballShape->getPosition(), ballShape->getRadius(), platformLines);

			if (line != platformLines.end()) {
				auto nextDirection = reflectVector(ballShape->getPosition(), ballShape->getRadius(), ball->GetDirection(), line->p1, line->p2);
				ball->SetDirection(nextDirection);
			}

			for (auto gameObject : gameObjects) {
				gameObject->Update(timeDelta);
			}
		}

		this->uiState.Update();
	}

	void State::Draw()
	{
		if (gameState.top() == GameState::Game)
		{
			gameField.Draw();
			player.Draw();

			for (auto gameObject : gameObjects) {
				gameObject->Draw();
			}
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
		gameState.push(GameState::MainMenu);

		actorsInfo[ActorType::APPLE].num = NUM_APPLES;
		actorsInfo[ActorType::APPLE].store.resize(NUM_APPLES);

		actorsInfo[ActorType::STONE].num = NUM_STONES;
		actorsInfo[ActorType::STONE].store.resize(NUM_STONES);

		actorsInfo[ActorType::BONUS].num = NUM_BONUSES;
		actorsInfo[ActorType::BONUS].store.resize(NUM_BONUSES);

		// TODO: move resources loading into separate class
		// Init game resources (terminate if error)
		assert(playerTexture.loadFromFile(RESOURCES_PATH + "Snake_Body.png"));
		assert(playerHeadTexture.loadFromFile(RESOURCES_PATH + "Snake_head.png"));
		assert(actorsInfo[ActorType::APPLE].texture.loadFromFile(RESOURCES_PATH + "Apple.png"));
		assert(actorsInfo[ActorType::STONE].texture.loadFromFile(RESOURCES_PATH + "Stone.png"));
		assert(actorsInfo[ActorType::BONUS].texture.loadFromFile(RESOURCES_PATH + "xyz-logo.png"));
		assert(font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		soundManager = new SoundManager({
			{Sounds::DeathSound, "Death.wav"},
			{Sounds::ApplePickSound, "AppleEat.wav"},
			{Sounds::BonusPickSound, "ding.flac"},
			},{
			{Music::Background, "Clinthammer__Background_Music.wav"},
			});

		xCellsNum = SCREEN_WIDTH / FIELD_CELL_SIZE;
		yCellsNum = (SCREEN_HEGHT - (unsigned int)TOP_PADDING) / FIELD_CELL_SIZE;

		platform = new Platform(playerHeadTexture);
		platform->SetSpeed(300.f);
		platform->Move({ (SCREEN_WIDTH - 180.f) / 2.f, SCREEN_HEGHT - 50.f}); //TODO: remove magic numbers

		gameObjects.push_back(platform);

		ball = new Ball(actorsInfo[ActorType::APPLE].texture);
		ball->Move({ (SCREEN_WIDTH - 10.f) / 2.f, SCREEN_HEGHT - 150.f }); //TODO: remove magic numbers
		ball->SetSpeed(250.f);
		ball->SetDirection({ 0.1f, 1.f });

		gameObjects.push_back(ball);
	}

	void State::Init(sf::RenderWindow& window)
	{
		*setings.getIsMusicOn() = true;
		*setings.getisSoundOn() = true;

		soundManager->SetLoop(Music::Background, true);
		soundManager->Play(Music::Background);

		difficulty = Difficulty::MEDIUM;
		GenerateRecordsList();

		uiState.InitUI(&window);
	}
}