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

	Difficulty* State::getDifficulty() { return &difficulty; }

	void State::setDifficulty(Difficulty value) { difficulty = value; }

	const stack<GameState>& State::getGameState() { return gameState; }

	void State::pushGameState(const GameState gs) { gameState.push(gs); }
	void State::popGameState() { gameState.pop(); }

	void State::clearGameState() { gameState = {}; }

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
				Restart();
				gameState.pop();
			}
			else if (gameState.top() == GameState::Game)
			{
				gameState.push(GameState::PauseMenu);
			}
			break;
		}
		case sf::Keyboard::F5: 
		{
			if (gameState.top() == GameState::Game) {
				auto gameSnapshot = gameStore->getGameSnapshot();
				gameCaretaker.setMemento(*gameSnapshot);
			}
			break;
		}
		case sf::Keyboard::F6:
		{
			if (gameState.top() == GameState::GameOverMenu && gameCaretaker.isSaveExists())
			{
				timeSinceGameOver = 0.f;
				gameObjects.clear();

				auto memento = gameCaretaker.getMemento();
				gameStore->restoreGameSnapshot(memento);
				gameState.pop();
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
		timeSinceGameOver = 0.f;

		gameStore->Restart(blockTexture);
	}

	void State::setGameOverState()
	{
		gameState.push(GameState::GameOverMenu);
		timeSinceGameOver = 0.f;

		auto prevRecord = recordsList.find(PLAYER_NAME);
		if (prevRecord->second < gameStore->getGameScore())
		{
			prevRecord->second = gameStore->getGameScore();
		}

		soundManager->Play(Sounds::DeathSound);
	}

	Font& State::GetFont() { return font; }

	int State::GetScore() { return gameStore->getGameScore(); }

	void State::Update(float timeDelta)
	{
		if (gameState.top() == GameState::Game)
		{
			auto result = gameStore->Update();

			bool hasDeleted = get<0>(result);
			string deletedId = get<1>(result);

			auto score = gameStore->getGameScore();

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

		sf::Image image;
		assert(image.loadFromFile(RESOURCES_PATH + "Cracks.png"));
		image.createMaskFromColor(sf::Color::White);
		assert(blockTexture.loadFromImage(image));
		blockTexture.setSmooth(true);

		soundManager = new SoundManager({
			{Sounds::DeathSound, "Death.wav"},
			{Sounds::ApplePickSound, "AppleEat.wav"},
			{Sounds::BonusPickSound, "ding.flac"},
			}, {
			{Music::Background, "Clinthammer__Background_Music.wav"},
			});

		gameStore = make_shared<GameStore>(gameObjects);
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