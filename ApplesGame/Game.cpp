#pragma once
#include <SFML/Audio.hpp>
#include "Game.h"
#include "GameSettings.h"
#include "Utils.h"
#include <assert.h>
#include <string>
#include <iostream>
#include <random>

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
		case GameState::WinPage:
			uiState.winPage->HandleKeyboardEvent(evt);
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
 		score = 0;
		timeSinceGameOver = 0.f;

		CreateNewBlockGrid();

		platform->Move({ (SCREEN_WIDTH - 180.f) / 2.f, SCREEN_HEGHT - 50.f }); //TODO: remove magic numbers
		platform->SetSpeed(300.f);

		ball->Move({ (SCREEN_WIDTH - 10.f) / 2.f, SCREEN_HEGHT - 150.f }); //TODO: remove magic numbers
		ball->SetSpeed(450.f);

		std::random_device rd; // Случайный генератор
		std::mt19937 gen(rd()); // Генератор псевдослучайных чисел Mersenne Twister
		std::uniform_real_distribution<float> dist(-0.7f, 0.7f);

		ball->SetDirection({ dist(gen), -1.f});
	}

	void State::setGameOverState()
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
			auto line = findIntersectionCircleRectangle(ball->GetPosition(), ballShape->getRadius(), platformLines);

			if (line != platformLines.end()) {
				auto nextDirection = reflectVector(ball->GetPosition(), ballShape->getRadius(), ball->GetDirection(), line->p1, line->p2);
				ball->SetDirection(nextDirection);
			}
			
			bool hasDeleted = false;
			pair<int, int> deletedIdx;

			auto grid = blocksGrid->GetGrid();
			for (int i = 0; i < grid.size(); ++i) {
				for (int j = 0; j < grid[i].size(); ++j) {
					auto block = grid[i][j];

					if (block == nullptr) {
						continue;
					}

					auto blockLines = GetRectLines(*block->GetShape(), block->GetPosition());
					auto line = findIntersectionCircleRectangle(ball->GetPosition(), ballShape->getRadius(), blockLines);

					if (line != blockLines.end()) {
						auto nextDirection = reflectVector(ball->GetPosition(), ballShape->getRadius(), ball->GetDirection(), line->p1, line->p2);
						ball->SetDirection(nextDirection);

						hasDeleted = true;
						deletedIdx = { i, j };
						break;
					}
				}
			}

			if (hasDeleted) {
				auto el = grid[deletedIdx.first][deletedIdx.second];
				gameObjects.erase(el->GetId());
				blocksGrid->RemoveEl(deletedIdx.first, deletedIdx.second);

				++score;
			}

			if (score == NUM_X * NUM_Y) {
				soundManager->Play(Sounds::BonusPickSound);
				gameState.push(GameState::WinPage);
			}
			else if (hasDeleted) {
				soundManager->Play(Sounds::ApplePickSound);
			}

			for (auto& gameObject : gameObjects) {
				gameObject.second->Update(timeDelta);
			}
		}

		this->uiState.Update();
	}

	void State::Draw()
	{
		uiState.Draw();

		if (gameState.top() == GameState::Game)
		{
			for (auto& gameObject : gameObjects) {
				gameObject.second->Draw();
			}
		}
	}

	void State::CreateNewBlockGrid() 
	{
		if (blocksGrid != nullptr) {
			for (auto& blocksColumn : blocksGrid->GetGrid()) {
				for (auto& block : blocksColumn) {
					gameObjects.erase(block->GetId());
				}
			}
		}

		delete blocksGrid;

		blocksGrid = new BlocksGrid();

		for (auto& blocksColumn : blocksGrid->GetGrid()) {
			for (auto& block : blocksColumn) {
				gameObjects[block->GetId()] = block;
			}
		}
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

		assert(font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		soundManager = new SoundManager({
			{Sounds::DeathSound, "Death.wav"},
			{Sounds::ApplePickSound, "AppleEat.wav"},
			{Sounds::BonusPickSound, "ding.flac"},
			},{
			{Music::Background, "Clinthammer__Background_Music.wav"},
			});

		platform = new Platform();
		ball = new Ball();

		gameObjects[platform->GetId()] = platform;
		gameObjects[ball->GetId()] = ball;
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