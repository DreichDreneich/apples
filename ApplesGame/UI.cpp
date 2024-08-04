#include <string>
#include "UI.h"
#include "MainMenu.h"
#include "Game.h"
#include "Menu.h"

using namespace std;
using namespace sf;

namespace ApplesGame
{
	void InitGameTitle(Text& gameTitle, const sf::Font& font)
	{
		gameTitle.setFont(font);
		gameTitle.setCharacterSize(48);
		gameTitle.setFillColor(sf::Color::Red);
		gameTitle.setStyle(sf::Text::Bold);
		gameTitle.setString("APPLES GAME");
		gameTitle.setOrigin(GetTextOrigin(gameTitle, { 0.5f, 0.5f }));
	}

	void InitRegularText(sf::Text& text, const sf::Font& font, const float xOrigin = 1.f)
	{
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setOrigin(GetTextOrigin(text, { xOrigin, 0.f }));
	}

	void InitRecordsList(UIState& uiState, const sf::Font& font)
	{
		uiState.recordsListHeader.setFont(font);
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
			InitRegularText(key, font);
			key.setString(it->first + " ");
			InitRegularText(value, font);

			it->second = { key, value };
			++it;
		}
	}

	void InitPauseGame(PauseGameMenu& pauseGameMenu, const sf::Font& font)
	{
		pauseGameMenu.mainMenu.setString("<1>  Main menu");
		InitRegularText(pauseGameMenu.mainMenu, font, 0.f);

		pauseGameMenu.quitGame.setString("<2>  Quit game");
		InitRegularText(pauseGameMenu.quitGame, font, 0.f);

		pauseGameMenu.resumeGame.setString("<3>  Resume game");
		InitRegularText(pauseGameMenu.resumeGame, font, 0.f);
	}

	void DrawRecordsList(const State& gameState, sf::RenderWindow& window, float topMargin = 0)
	{
		UIState uiState = gameState.uiState;
		uiState.recordsListHeader.setPosition(window.getSize().x / 2.f, topMargin + 100);

		window.draw(uiState.recordsListHeader);

		vector<pair<string, int>> pairs(gameState.recordsList.begin(), gameState.recordsList.end());
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

		DrawRecordsList(gameState, window, gameOverTextY);

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

	void UIState::InitUI(const sf::Font& font)
	{
		InitGameTitle(gameTitle, font);

		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Yellow);

		inputHintText.setFont(font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::White);
		inputHintText.setString("Use arrow keys to move, Space to restart, ESC to exit");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));

		gameOverText.setFont(font);
		gameOverText.setCharacterSize(48);
		gameOverText.setStyle(sf::Text::Bold);
		gameOverText.setFillColor(sf::Color::Red);
		gameOverText.setString("GAME OVER");
		gameOverText.setOrigin(GetTextOrigin(gameOverText, { 0.5f, 0.5f }));

		isBonusDurationVisible = false;
		bonusDuration.setFont(font);
		bonusDuration.setCharacterSize(14);
		bonusDuration.setFillColor(sf::Color::White);

		menuPage = *new MenuPage(font);

		InitMainMenu(mainMenu, font);
		InitRecordsList(*this, font);
		InitPauseGame(pauseGameMenu, font);
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
		UIState uiState = state.uiState;

		switch (state.gameState.top())
		{
		case GameState::MainMenu:
		{
			uiState.menuPage.Draw(window);

			uiState.gameTitle.setPosition((float)window.getSize().x / 2, 60.f);
			window.draw(uiState.gameTitle);
			auto i = uiState.gameTitle.getString().toAnsiString();

			//DrawMainMenu(uiState.mainMenu, state.gameMode, window);
			break;
		}
		case GameState::GameOverMenu:
		{
			DrawGameOverScreen(state, window);

			DrawHint(uiState, window);

			break;
		}
		case GameState::Game:
		{
			if (uiState.isBonusDurationVisible) 
			{
				uiState.bonusDuration.setPosition(uiState.bonusDurationPosition);
				window.draw(uiState.bonusDuration);
			}

			uiState.scoreText.setPosition(10.f, 10.f);
			window.draw(uiState.scoreText);

			uiState.inputHintText.setPosition(window.getSize().x - 10.f, 10.f);
			window.draw(uiState.inputHintText);

			break;
		}
		case GameState::Records:
		{
			DrawRecordsList(state, window);

			break;
		}
		case GameState::PauseMenu:
		{
			DrawPauseMenu(uiState, window);

			break;
		}
		}
	}
}

