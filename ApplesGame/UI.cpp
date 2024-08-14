#pragma once

#include <string>
#include "UI.h"
#include "MainMenu.h"
#include "Game.h"
#include "Menu.h"
#include "RecordsListPage.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	void InitGameTitle(Text& gameTitle)
	{
		gameTitle.setFont(State::Instance()->font);
		gameTitle.setCharacterSize(48);
		gameTitle.setFillColor(sf::Color::Red);
		gameTitle.setStyle(sf::Text::Bold);
		gameTitle.setString("APPLES GAME");
		gameTitle.setOrigin(GetTextOrigin(gameTitle, { 0.5f, 0.5f }));
	}

	void InitRegularText(sf::Text& text, const float xOrigin = 1.f)
	{
		text.setFont(State::Instance()->font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setOrigin(GetTextOrigin(text, { xOrigin, 0.f }));
	}

	void InitPauseGame(PauseGameMenu& pauseGameMenu)
	{
		pauseGameMenu.mainMenu.setString("<1>  Main menu");
		InitRegularText(pauseGameMenu.mainMenu, 0.f);

		pauseGameMenu.quitGame.setString("<2>  Quit game");
		InitRegularText(pauseGameMenu.quitGame, 0.f);

		pauseGameMenu.resumeGame.setString("<3>  Resume game");
		InitRegularText(pauseGameMenu.resumeGame, 0.f);
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

		scoreText.setFont(State::Instance()->font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Black);
		scoreText.setStyle(sf::Text::Bold);

		inputHintText.setFont(State::Instance()->font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::Black);
		inputHintText.setString("Use arrow keys to move, Space to restart, ESC to exit");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));

		gameOverText.setFont(State::Instance()->font);
		gameOverText.setCharacterSize(48);
		gameOverText.setStyle(sf::Text::Bold);
		gameOverText.setFillColor(sf::Color::Red);
		gameOverText.setString("GAME OVER");
		gameOverText.setOrigin(GetTextOrigin(gameOverText, { 0.5f, 0.5f }));

		isBonusDurationVisible = false;
		bonusDuration.setFont(State::Instance()->font);
		bonusDuration.setCharacterSize(14);
		bonusDuration.setFillColor(sf::Color::White);

		menuPage.Init();
		difficultyPage->Init();

		InitPauseGame(pauseGameMenu);
	}

	void UIState::Update()
	{
		scoreText.setString("Score: " + std::to_string(State::Instance()->score));

		sf::Color gameOverTextColor = (int)State::Instance()->timeSinceGameOver % 2 ? sf::Color::Red : sf::Color::Yellow;
		gameOverText.setFillColor(gameOverTextColor);

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

			/*gameTitle.setPosition((float)window->getSize().x / 2, 60.f);
			window->draw(gameTitle);*/

			break;
		}
		case GameState::DifficultyPage:
		{
			difficultyPage->Draw();
			break;
		}
		case GameState::GameOverMenu:
		{
			gameOverPage = make_unique<GameOverPage>();
			gameOverPage->Draw();

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

			inputHintText.setPosition(window->getSize().x - 10.f, 10.f);
			window->draw(inputHintText);

			break;
		}
		case GameState::Records:
		{
			recordsList1 = make_unique<RecordsList>();
			recordsList1->Draw();

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

