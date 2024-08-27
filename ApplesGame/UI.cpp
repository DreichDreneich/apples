#pragma once

#include <string>
#include "UI.h"
#include "MainMenu.h"
#include "Game.h"
#include "Menu.h"
#include "RecordsListPage.h"
#include "SettingsPage.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	void InitGameTitle(Text& gameTitle)
	{
		gameTitle.setFont(State::Instance()->GetFont());
		gameTitle.setCharacterSize(48);
		gameTitle.setFillColor(sf::Color::Red);
		gameTitle.setStyle(sf::Text::Bold);
		string str = "Змейка";
		gameTitle.setString(String::fromUtf8(str.begin(), str.end()));
		gameTitle.setOrigin(GetTextOrigin(gameTitle, { 0.5f, 0.5f }));
	}

	// TODO: use TextComponent
	void InitRegularText(sf::Text& text, const string str)
	{
		text.setFont(State::Instance()->GetFont());
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setOrigin(GetTextOrigin(text, { 0.f, 0.f }));
		text.setString(sf::String::fromUtf8(str.begin(), str.end()));
	}

	void InitPauseGame(PauseGameMenu& pauseGameMenu)
	{
		InitRegularText(pauseGameMenu.mainMenu, "<1> Главное меню");
		InitRegularText(pauseGameMenu.quitGame, "<2> Выйти из игры");
		InitRegularText(pauseGameMenu.resumeGame, "<3> Продолжить игру");
	}
	
	void UIState::DrawHint()
	{
		inputHintText.setPosition(window->getSize().x - 10.f, 10.f);
		window->draw(inputHintText);
	}

	void UIState::DrawPauseMenu()
	{
		//TODO: find longest string
		auto mainMenuItemWidth = pauseGameMenu.resumeGame.getLocalBounds().width / 2;

		float windowX = (float)window->getSize().x / 2;
		float windowY = (float)window->getSize().y / 3;

		gameTitle.setPosition(windowX, 60.f);

		auto xWithOffset = windowX - mainMenuItemWidth;

		pauseGameMenu.mainMenu.setPosition(xWithOffset, windowY);
		pauseGameMenu.quitGame.setPosition(xWithOffset, windowY + 35.f);
		pauseGameMenu.resumeGame.setPosition(xWithOffset, windowY + 70.f);
		
		window->draw(pauseGameMenu.mainMenu);
		window->draw(pauseGameMenu.quitGame);
		window->draw(pauseGameMenu.resumeGame);
		window->draw(gameTitle);
	}

	UIState::UIState()
	{
		difficultyPage = new DifficultyPage();
	}

	void UIState::InitUI(sf::RenderWindow* window)
	{
		UIState::window = window;

		InitGameTitle(gameTitle);

		scoreText.setFont(State::Instance()->GetFont());
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Black);
		scoreText.setStyle(sf::Text::Bold);

		inputHintText.setFont(State::Instance()->GetFont());
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::Black);
		string hintStr = "Используйте стрелки что бы двигаться,\nSpace для паузы, ESC для выхода";
		inputHintText.setString(String::fromUtf8(hintStr.begin(), hintStr.end()));
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));

		isBonusDurationVisible = false;
		bonusDuration.setFont(State::Instance()->GetFont());
		bonusDuration.setCharacterSize(14);
		bonusDuration.setFillColor(sf::Color::White);

		menuPage.Init();
		difficultyPage->Init();
		gameOverPage = make_unique<GameOverPage>();
		recordsList = make_unique<RecordsList>();
		settingsPage = make_unique<SettingsPage>();

		InitPauseGame(pauseGameMenu);
	}

	void UIState::Update()
	{
		string scoreStr = "Счет: ";
		scoreText.setString(String::fromUtf8(scoreStr.begin(), scoreStr.end()) + std::to_string(State::Instance()->GetScore()));

		sf::Color gameOverTextColor = (int)State::Instance()->timeSinceGameOver % 2 ? sf::Color::Red : sf::Color::Yellow;

		isBonusDurationVisible = State::Instance()->getPlayer()->hasBonus;
		bonusDuration.setString(std::to_string((int)State::Instance()->getPlayer()->bonusTimeRemaining));
		bonusDurationPosition.x = State::Instance()->getPlayer()->position.x;
		bonusDurationPosition.y = State::Instance()->getPlayer()->position.y - State::Instance()->getPlayer()->size / 2;
	}

	void UIState::Draw()
	{
		switch (State::Instance()->getGameState()->top())
		{
		case GameState::MainMenu:
		{
			menuPage.Draw();

			gameTitle.setPosition((float)window->getSize().x / 2, 100.f);
			window->draw(gameTitle);

			break;
		}
		case GameState::DifficultyPage:
		{
			difficultyPage->Draw();
			break;
		}
		case GameState::GameOverMenu:
		{
			gameOverPage->Draw();
			break;
		}
		case GameState::SettingsPage:
		{
			settingsPage->Draw();
			break;
		}
		case GameState::Game:
		{
			if (isBonusDurationVisible) 
			{
				bonusDuration.setPosition(bonusDurationPosition);
				window->draw(bonusDuration);
			}

			scoreText.setPosition(10.f, 10.f);
			window->draw(scoreText);

			inputHintText.setPosition(window->getSize().x - 10.f, 3.f);
			window->draw(inputHintText);

			break;
		}
		case GameState::Records:
		{
			recordsList->Draw();
			break;
		}
		case GameState::PauseMenu:
		{
			DrawPauseMenu();
			break;
		}
		}
	}
}

