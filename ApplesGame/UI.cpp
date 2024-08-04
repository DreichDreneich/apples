#pragma once

#include <string>
#include "UI.h"
#include "MainMenu.h"
#include "Game.h"
#include "Menu.h"

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

	void InitRecordsList(UIState& uiState)
	{
		uiState.recordsListHeader.setFont(State::Instance()->font);
		uiState.recordsListHeader.setCharacterSize(32);
		uiState.recordsListHeader.setStyle(sf::Text::Bold);
		uiState.recordsListHeader.setFillColor(sf::Color::Yellow);
		uiState.recordsListHeader.setString("Records List");
		uiState.recordsListHeader.setOrigin(GetTextOrigin(uiState.recordsListHeader, { 0.5f, 0.5f }));

		auto it = uiState.recordsList.begin();

		while (it != uiState.recordsList.end())
		{
			Text key;
			Text value;
			InitRegularText(key);
			key.setString(it->first + " ");
			InitRegularText(value);

			it->second = { key, value };
			++it;
		}
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

	void DrawRecordsList(sf::RenderWindow& window, float topMargin = 0)
	{
		auto gameState = State::Instance();
		UIState uiState = gameState->uiState;
		uiState.recordsListHeader.setPosition(window.getSize().x / 2.f, topMargin + 100);

		window.draw(uiState.recordsListHeader);

		vector<pair<string, int>> pairs(gameState->recordsList.begin(), gameState->recordsList.end());
		std::sort(pairs.begin(), pairs.end(), [](pair<string, int> a, pair<string, int> b) {
			return a.second > b.second;
			});

		for (short i = 0; i < pairs.size(); ++i)
		{
			auto recordTexts = uiState.recordsList[pairs[i].first];

			recordTexts.second.setString(to_string(pairs[i].second));

			auto recordY = topMargin + 150 + i * 50;

			recordTexts.first.setPosition(window.getSize().x / 2.f - 100, recordY);
			recordTexts.second.setPosition(window.getSize().x / 2.f + 100, recordY);

			window.draw(recordTexts.first);
			window.draw(recordTexts.second);
		}
	}

	void DrawGameOverScreen(const State& gameState, sf::RenderWindow& window)
	{
		UIState uiState = gameState.uiState;

		auto gameOverTextY = window.getSize().y / 5.f;
		uiState.gameOverText.setPosition(window.getSize().x / 2.f, gameOverTextY);

		sf::RectangleShape background;
		background.setPosition(0.f, 0.f);
		background.setSize({ (float)window.getSize().x, (float)window.getSize().y });
		background.setFillColor({ 0, 0, 0, 200 });

		window.draw(background);

		DrawRecordsList(window, gameOverTextY);

		window.draw(uiState.gameOverText);
	}
	
	void DrawHint(UIState& uiState, sf::RenderWindow& window)
	{
		uiState.inputHintText.setPosition(window.getSize().x - 10.f, 10.f);
		window.draw(uiState.inputHintText);
	}

	void DrawPauseMenu(UIState& uiState, sf::RenderWindow& window)
	{
		auto pauseMenu = uiState.pauseGameMenu;
		//TODO: find longest string
		auto mainMenuItemWidth = pauseMenu.resumeGame.getLocalBounds().width / 2;

		float windowX = (float)window.getSize().x / 2;
		float windowY = (float)window.getSize().y / 3;

		uiState.gameTitle.setPosition(windowX, 60.f);

		auto xWithOffset = windowX - mainMenuItemWidth;

		pauseMenu.mainMenu.setPosition(xWithOffset, windowY);
		pauseMenu.quitGame.setPosition(xWithOffset, windowY + 35.f);
		pauseMenu.resumeGame.setPosition(xWithOffset, windowY + 70.f);
		
		window.draw(pauseMenu.mainMenu);
		window.draw(pauseMenu.quitGame);
		window.draw(pauseMenu.resumeGame);
		window.draw(uiState.gameTitle);
	}

	UIState::UIState()
	{
	}

	void UIState::InitUI()
	{
		InitGameTitle(gameTitle);

		scoreText.setFont(State::Instance()->font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Yellow);

		inputHintText.setFont(State::Instance()->font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::White);
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

		InitMainMenu(mainMenu);
		InitRecordsList(*this);
		InitPauseGame(pauseGameMenu);
	}

	void UIState::Update(const struct State& state)
	{
		scoreText.setString("Score: " + std::to_string(state.score));

		sf::Color gameOverTextColor = (int)state.timeSinceGameOver % 2 ? sf::Color::Red : sf::Color::Yellow;
		gameOverText.setFillColor(gameOverTextColor);

		isBonusDurationVisible = state.player.hasBonus;
		bonusDuration.setString(std::to_string((int)state.player.bonusTimeRemaining));
		bonusDurationPosition.x = state.player.position.x;
		bonusDurationPosition.y = state.player.position.y - state.player.size / 2;
	}

	void UIState::Draw(const State& state, sf::RenderWindow& window)
	{
		switch (state.gameState.top())
		{
		case GameState::MainMenu:
		{
			menuPage.Draw(window);

			gameTitle.setPosition((float)window.getSize().x / 2, 60.f);
			window.draw(gameTitle);

			//DrawMainMenu(uiState.mainMenu, state.gameMode, window);
			break;
		}
		case GameState::GameOverMenu:
		{
			DrawGameOverScreen(state, window);

			DrawHint(*this, window);

			break;
		}
		case GameState::Game:
		{
			if (isBonusDurationVisible) 
			{
				bonusDuration.setPosition(bonusDurationPosition);
				window.draw(bonusDuration);
			}

			scoreText.setPosition(10.f, 10.f);
			window.draw(scoreText);

			inputHintText.setPosition(window.getSize().x - 10.f, 10.f);
			window.draw(inputHintText);

			break;
		}
		case GameState::Records:
		{
			DrawRecordsList(window);

			break;
		}
		case GameState::PauseMenu:
		{
			DrawPauseMenu(*this, window);

			break;
		}
		}
	}
}

